#include "super_cycle.h"
#include "main.h"
#include "platform_events.h"
#include "platform_tick.h"
#include "bsp_gpio.h"
#include "adg888.h"
#include "ad7799.h"
#include "usb_ft232.h"
#include "measure_fsm.h"

#define SC_ENABLE_MEASURE    1
#define SC_TIMING_DEMO       0  /* каркас диаграммы больше не нужен */
#define SC_DEBUG_HEARTBEAT   0	/* заменяется печатью результата */
#define SC_DEBUG_MEASURE     1
#define SC_DEBUG_DIAG        1   /* ВРЕМЕННО: строка [D] раз в 1 с */
#define SC_DIAG_SPI          1   /* ВРЕМЕННО: печатать состояние AD7799 в середине цикла */

#if (SC_DEBUG_MEASURE != 0)
static uint32_t AvddToMv(int32_t code);
static void     Debug_PrintMeasure(const MeasureResult_t *r);
#endif

/* Заглушки модулей Этапов 4..7. Слабые символы перекрываются реальными
   реализациями без единой правки планировщика. */
__attribute__((weak)) void Heater_Task(void) { }
__attribute__((weak)) void Status_Task(void) { }
__attribute__((weak)) void Comms_Task(void)  { }
__attribute__((weak)) void Usb_Task(void)    { }
__attribute__((weak)) void Log_Task(void)    { }
__attribute__((weak)) void Measurement_OnResult(void) { }

static uint32_t s_cycle_count = 0U;
static uint32_t s_max_cycles  = 0U;
static uint8_t  s_meas_active = 0U;

static void Task_Led(void);
static void Profile_Init(void);

/* --- Реестр фоновых задач (ТЗ п.7.1.a) --- */
typedef struct {
    void          (*fn)(void);
    PlatformEvent_t event;
} TaskItem_t;

static const TaskItem_t s_registry[] = {
    { Comms_Task,  PLATFORM_EV_UART3_IDLE },
    { Usb_Task,    PLATFORM_EV_USB_RX     },
    { Heater_Task, PLATFORM_EV_1HZ        },
    { Status_Task, PLATFORM_EV_1HZ        },
    { Log_Task,    PLATFORM_EV_1HZ        },
    { Task_Led,    PLATFORM_EV_100MS      }
};
#define SC_TASK_COUNT  (sizeof(s_registry) / sizeof(s_registry[0]))

static void Profile_Init(void)
{
    /* DWT CYCCNT — счётчик тактов ядра для замера отзывчивости планировщика */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
}

void SuperCycle_Init(void)
{
    Profile_Init();
    s_cycle_count = 0U;
    s_max_cycles  = 0U;
    s_meas_active = 0U;
}

#if (SC_DEBUG_MEASURE != 0)
/* Код AVDD-монитора -> милливольты: code/2^24 * 1.17 В * 6 = code * 7020 / 2^24 */
static uint32_t AvddToMv(int32_t code)
{
    if (code <= 0) { return 0U; }
    return (uint32_t)(((uint64_t)code * 7020U) >> 24);
}

static void Debug_PrintMeasure(const MeasureResult_t *r)
{
    USB_FT232_Printf("[M] c=%lu dur=%lu fwd=%06lX rev=%06lX b6=%06lX b8=%06lX b7=%06lX avdd=%lumV/%lumV\r\n",
        (unsigned long)MeasureFSM_GetCycleCount(),
        (unsigned long)(r->timestamp - r->cycle_start),
        (unsigned long)r->w_fwd, (unsigned long)r->w_rev,
        (unsigned long)r->b6, (unsigned long)r->b8, (unsigned long)r->b7,
        (unsigned long)AvddToMv(r->avdd_d22), (unsigned long)AvddToMv(r->avdd_d23));

    USB_FT232_Printf("[M] rdy=%u,%u,%u,%u,%u,%u,%u st=%02X,%02X,%02X,%02X,%02X,%02X,%02X mode0=%04X valid=%02X err=%08lX ovr=%lu\r\n",
        r->step_ms[0], r->step_ms[1], r->step_ms[2], r->step_ms[3],
        r->step_ms[4], r->step_ms[5], r->step_ms[6],
        r->step_status[0], r->step_status[1], r->step_status[2], r->step_status[3],
        r->step_status[4], r->step_status[5], r->step_status[6],
        r->step_mode[0], r->valid_mask,
        (unsigned long)r->error_mask, (unsigned long)MeasureFSM_GetOverrunCount());
}
#endif

void SuperCycle_Run(void)
{
    uint32_t t_start = DWT->CYCCNT;
    uint32_t elapsed;
    uint8_t  pending[PLATFORM_EV_COUNT];
    uint32_t i;

    /* 1. Старт цикла от мастер-таймера TIM4 */
    if (Events_Consume(PLATFORM_EV_CYCLE_6S) != 0U) {
        s_cycle_count++;
        MeasureFSM_Start(Events_GetCycleStartTick());
    }

    /* 2. Измерительный FSM — прямой вызов вне реестра (ТЗ п.7.2) */
    MeasureFSM_Task();
    s_meas_active = MeasureFSM_IsActive();

    if (MeasureFSM_IsDataReady() != 0U) {
#if (SC_DEBUG_MEASURE != 0)
        Debug_PrintMeasure(MeasureFSM_GetResultPtr());
#endif
        Measurement_OnResult();   /* далее: weight_math -> accu_logic -> log */
    }

    /* 3. Снимок флагов: одно событие обслуживает несколько задач */
    for (i = 0U; i < (uint32_t)PLATFORM_EV_COUNT; i++) {
        pending[i] = Events_Consume((PlatformEvent_t)i);
    }

    /* 4. Фоновые задачи по расписанию */
    for (i = 0U; i < SC_TASK_COUNT; i++) {
        if (pending[s_registry[i].event] != 0U) {
            s_registry[i].fn();
        }
    }

    /* 5. Профиль отзывчивости */
    elapsed = DWT->CYCCNT - t_start;
    if (elapsed > s_max_cycles) {
        s_max_cycles = elapsed;
    }


}


/* Task_LED, ТЗ 2.10.2. Переедет в App/System/sys_led.c на Этапе 7. */
static void Task_Led(void)
{
    static uint8_t s_div10 = 0U;

    s_div10++;
    if (s_div10 >= 10U) { s_div10 = 0U; }

    BSP_LED_V15_Set((USB_FT232_GetState() == USB_CONNECTED) ? 1U : 0U);

    if (s_meas_active != 0U) {
        BSP_LED_V16_Toggle();                          /* идёт измерение */
    } else {
        BSP_LED_V16_Set((s_div10 == 0U) ? 1U : 0U);    /* «сердцебиение» 1 Гц */
    }
}

uint32_t SuperCycle_GetCycleCount(void) { return s_cycle_count; }

uint32_t SuperCycle_GetMaxLoopUs(void)
{
    return s_max_cycles / (SystemCoreClock / 1000000U);
}
