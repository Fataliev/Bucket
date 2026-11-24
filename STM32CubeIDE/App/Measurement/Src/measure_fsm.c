#include "measure_fsm.h"
#include "ad7799.h"
#include "adg888.h"
#include "bsp_gpio.h"
#include "platform_tick.h"
#include "main.h"          /* CMSIS: __get_PRIMASK / __disable_irq */
#include <string.h>

/* --- Временная диаграмма ТЗ 2.2.1 --- */
#define MEAS_SLOT_MS          480U    /* длительность одного шага */
#define MEAS_SWITCH_MS        1920U   /* переключение ADG888      */
#define MEAS_ACTIVE_MS        3360U   /* конец активной фазы PSW  */
#define MEAS_RDY_DEADLINE_MS  600U    /* крайний срок готовности внутри слота */

/* REF_DET для ратиометрических каналов. 0 — если потребуется вернуть
   конфигурацию точно к отлаженному baseline. */
#define MEAS_USE_REF_DET      1U

#if (MEAS_USE_REF_DET != 0)
#define MEAS_REFDET           AD7799_CONF_REF_DET
#else
#define MEAS_REFDET           0U
#endif

typedef enum {
    SUB_WAIT_SLOT = 0,   /* ждём наступления времени слота */
    SUB_WAIT_RDY         /* преобразование запущено, опрашиваем ~RDY */
} MeasSubState_t;

typedef struct {
    AD7799_Device_t dev;
    uint16_t        config;
    uint16_t        start_ms;
} MeasStepDesc_t;

/* Порядок и конфигурации строго по диаграмме.
   Шаг 0 намеренно не зависит от моста: пока идёт его преобразование,
   питание тензомоста от LT3092 успевает установиться. */
static const MeasStepDesc_t s_steps[MEAS_STEP_COUNT] = {
    { AD7799_DEV_1, (uint16_t)(AD7799_CONF_UNIPOLAR | AD7799_CONF_CH_AVDD_MON),                                  0U },
    { AD7799_DEV_1, (uint16_t)(AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(0) | AD7799_CONF_CH_AIN3 | MEAS_REFDET),  480U },
    { AD7799_DEV_1, (uint16_t)(AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(0) | AD7799_CONF_CH_AIN2 | MEAS_REFDET),  960U },
    { AD7799_DEV_1, (uint16_t)(AD7799_CONF_BIPOLAR  | AD7799_CONF_BUF | AD7799_CONF_GAIN(7) | AD7799_CONF_CH_AIN1 | MEAS_REFDET), 1440U },
    { AD7799_DEV_2, (uint16_t)(AD7799_CONF_BIPOLAR  | AD7799_CONF_BUF | AD7799_CONF_GAIN(7) | AD7799_CONF_CH_AIN1 | MEAS_REFDET), 1920U },
    { AD7799_DEV_2, (uint16_t)(AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(0) | AD7799_CONF_CH_AIN2 | MEAS_REFDET), 2400U },
    { AD7799_DEV_2, (uint16_t)(AD7799_CONF_UNIPOLAR | AD7799_CONF_CH_AVDD_MON),                                 2880U }
};

static MeasureResult_t s_work;        /* заполняется в текущем цикле   */
static MeasureResult_t s_published;   /* последний завершённый цикл    */

static uint8_t  s_active        = 0U;
static uint8_t  s_data_ready    = 0U;
static uint8_t  s_switched      = 0U;
static uint8_t  s_step          = 0U;
static MeasSubState_t s_sub     = SUB_WAIT_SLOT;
static uint32_t s_cycle_start   = 0U;
static uint32_t s_step_start    = 0U;
static uint32_t s_last_poll_ms  = 0U;
static uint32_t s_cycle_count   = 0U;
static uint32_t s_overrun_count = 0U;

/* ------------------------------------------------------------------------- */

static void Meas_StoreCode(uint8_t idx, int32_t code)
{
    switch (idx) {
        case MEAS_STEP_AVDD_D22: s_work.avdd_d22 = code; break;
        case MEAS_STEP_B8:       s_work.b8       = code; break;
        case MEAS_STEP_B6:       s_work.b6       = code; break;
        case MEAS_STEP_W_FWD:    s_work.w_fwd    = code; break;
        case MEAS_STEP_W_REV:    s_work.w_rev    = code; break;
        case MEAS_STEP_B7:       s_work.b7       = code; break;
        case MEAS_STEP_AVDD_D23: s_work.avdd_d23 = code; break;
        default: break;
    }
}

static void Meas_MarkTimeout(uint8_t idx)
{
    s_work.error_mask |= ((uint32_t)1U << idx);
    s_work.valid_mask &= (uint8_t)~((uint8_t)1U << idx);
    Meas_StoreCode(idx, 0);
}

/* Завершение активной фазы: 3360 мс. Выполняется ровно один раз за цикл. */
static void Meas_Finish(void)
{
    uint8_t i;

    /* Если какие-то шаги не успели выполниться — помечаем их недостоверными */
    for (i = s_step; i < (uint8_t)MEAS_STEP_COUNT; i++) {
        Meas_MarkTimeout(i);
    }

    AD7799_SetPSW(AD7799_DEV_1, 0U);
    AD7799_SetPSW(AD7799_DEV_2, 0U);

    /* Ключи в безопасное состояние: D22 на тензомосте, D23 на AGND */
    ADG888_SetPolarity(ADG888_POLARITY_FORWARD);

    s_work.timestamp   = GetTick();
    s_work.cycle_start = s_cycle_start;

    s_published  = s_work;      /* публикация одним присваиванием: FSM и читатели в одном контексте */
    s_data_ready = 1U;
    s_active     = 0U;
    s_cycle_count++;
}

/* ------------------------------------------------------------------------- */

void MeasureFSM_Init(void)
{
    AD7799_Init();                       /* блокирующий сброс — только на старте, до суперцикла */
    AD7799_SetPSW(AD7799_DEV_1, 0U);
    AD7799_SetPSW(AD7799_DEV_2, 0U);
    ADG888_SetPolarity(ADG888_POLARITY_FORWARD);

    memset(&s_work,      0, sizeof(s_work));
    memset(&s_published, 0, sizeof(s_published));

    s_active        = 0U;
    s_data_ready    = 0U;
    s_cycle_count   = 0U;
    s_overrun_count = 0U;
}

void MeasureFSM_Start(uint32_t cycle_start_tick)
{
    if (s_active != 0U) {
        s_overrun_count++;               /* предыдущий цикл не успел — не прерываем его */
        return;
    }

    memset(&s_work, 0, sizeof(s_work));

    s_cycle_start  = cycle_start_tick;
    s_step         = 0U;
    s_sub          = SUB_WAIT_SLOT;
    s_switched     = 0U;
    s_last_poll_ms = GetTick();

    ADG888_SetPolarity(ADG888_POLARITY_FORWARD);

    /* PSW обоих АЦП включаем сразу и держим все 3360 мс.
       Неактивный АЦП отключён ключом D20, поэтому лишнего тока нет;
       зато в момент 1920 мс питание моста не прерывается ни на микросекунду. */
    AD7799_SetPSW(AD7799_DEV_1, 1U);
    AD7799_SetPSW(AD7799_DEV_2, 1U);

    s_active = 1U;
}

void MeasureFSM_Task(void)
{
    uint32_t now;
    uint32_t phase;
    uint32_t primask;
    const MeasStepDesc_t *step;

    if (s_active == 0U) {
        return;
    }

    now   = GetTick();
    phase = (uint32_t)(now - s_cycle_start);

    /* 1. Переключение полярности строго на 1920 мс, в критической секции, без паузы */
    if ((s_switched == 0U) && (phase >= MEAS_SWITCH_MS)) {
        primask = __get_PRIMASK();
        __disable_irq();
        ADG888_SetPolarity(ADG888_POLARITY_REVERSE);
        __set_PRIMASK(primask);
        s_switched = 1U;
    }

    /* 2. Завершение активной фазы строго на 3360 мс */
    if (phase >= MEAS_ACTIVE_MS) {
        Meas_Finish();
        return;
    }

    if (s_step >= (uint8_t)MEAS_STEP_COUNT) {
        return;
    }
    step = &s_steps[s_step];

    /* 3. Старт преобразования по расписанию */
    if (s_sub == SUB_WAIT_SLOT) {
        if (phase >= (uint32_t)step->start_ms) {
            AD7799_StartConversion(step->dev, step->config, 1U);
            s_step_start   = GetTick();
            s_last_poll_ms = s_step_start;
            s_sub          = SUB_WAIT_RDY;
        }
        return;
    }

    /* 4. Ожидание завершения конвертации */
    if (s_sub == SUB_WAIT_RDY) {
        // 1. Ждем минимальные 480 мс
        if ((uint32_t)(now - s_step_start) < 480) {
            return;
        }

        // 2. Проверяем пин ~RDY. Если он высокий — АЦП еще думает.
        if (AD7799_IsReady(step->dev) == 0U) {
            // Ждем до 600 мс (запас 120 мс на всякий случай)
            if ((uint32_t)(now - s_step_start) < 600) {
                return;
            }
            // Если и за 600 мс не дождались — фиксируем таймаут
        }

        // 3. Читаем данные
        uint32_t code   = 0U;
        uint8_t  status = 0U;
        uint8_t  ok     = AD7799_ReadResult(step->dev, &code, &status);

        s_work.step_ms[s_step]     = (uint16_t)(GetTick() - s_step_start);
        s_work.step_status[s_step] = status;

        if ((status & AD7799_ST_NOREF) != 0U) {
            s_work.error_mask |= ((uint32_t)1U << (s_step + 8U));
        }
        if ((status & AD7799_ST_ERR) != 0U) {
            s_work.error_mask |= ((uint32_t)1U << (s_step + 16U));
        }

        if (ok != 0U) {
            s_work.valid_mask |= (uint8_t)((uint8_t)1U << s_step);
            Meas_StoreCode(s_step, (int32_t)code);
        } else {
            // Если ошибка (например, NOREF), пишем 0
            Meas_StoreCode(s_step, 0);
        }

        s_step++;
        s_sub = SUB_WAIT_SLOT;
        return;
    }

    /* 5. Таймаут внутри слота: данных нет, шаг недостоверен, цикл продолжаем.
       0xFE = конверсия запущена, но ~RDY не опустился (проблема шины/питания).
       0xFF = конверсия не запускалась вообще. */
    if ((uint32_t)(now - s_step_start) >= MEAS_RDY_DEADLINE_MS) {
        s_work.step_ms[s_step]     = MEAS_RDY_DEADLINE_MS;
        s_work.step_status[s_step] = 0xFEU;
        Meas_MarkTimeout(s_step);
        s_step++;
        s_sub = SUB_WAIT_SLOT;
    }
}

void MeasureFSM_Reset(void)
{
    AD7799_SetPSW(AD7799_DEV_1, 0U);
    AD7799_SetPSW(AD7799_DEV_2, 0U);
    ADG888_SetPolarity(ADG888_POLARITY_FORWARD);
    s_active     = 0U;
    s_data_ready = 0U;
    s_step       = 0U;
    s_sub        = SUB_WAIT_SLOT;
}

uint8_t MeasureFSM_IsActive(void) { return s_active; }

uint8_t MeasureFSM_IsDataReady(void)
{
    if (s_data_ready != 0U) {
        s_data_ready = 0U;
        return 1U;
    }
    return 0U;
}

MeasureResult_t        MeasureFSM_GetResult(void)    { return s_published; }
const MeasureResult_t *MeasureFSM_GetResultPtr(void) { return &s_published; }
uint32_t               MeasureFSM_GetCycleCount(void){ return s_cycle_count; }
uint32_t               MeasureFSM_GetOverrunCount(void) { return s_overrun_count; }
