#include "measure_fsm.h"
#include "ad7799.h"
#include "bsp_gpio.h"
#include "platform_tick.h"

#define MFSM_SETTLE_MS   5     // settling моста/АЦП (ТЗ п.3)
#define MFSM_CONV_MS     480   // sinc3 @4.17 Гц
#define MFSM_RDY_TMO     600   // таймаут RDY с запасом

// Таблица 7 шагов (ТЗ п.2.2.1 + Прил.В). Прил.А ТЗ противоречит тексту —
// gain выбран по схеме: вес=128, RTD=1, int.temp=1.
static const MFSM_Step_t steps[7] = {
    // M1 D22 int temp (bipolar, gain1)
    {AD7799_CH1, 0, AD7799_CONF_BIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(0) | AD7799_CH_INT_TEMP}, // 0x0017
    // M2 D22 AIN3 (B8) unipolar gain1
    {AD7799_CH1, 0, AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(0) | AD7799_CH_AIN3},    // 0x1012
    // M3 D22 AIN2 (B6) unipolar gain1
    {AD7799_CH1, 0, AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(0) | AD7799_CH_AIN2},    // 0x1011
    // M4 D22 AIN1 (вес fwd) unipolar gain128
    {AD7799_CH1, 0, AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(7) | AD7799_CH_AIN1},    // 0x1710
    // M5 D23 AIN1 (вес rev) unipolar gain128, SW=HIGH
    {AD7799_CH2, 1, AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(7) | AD7799_CH_AIN1},
    // M6 D23 AIN2 (B7) unipolar gain1, SW=HIGH
    {AD7799_CH2, 1, AD7799_CONF_UNIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(0) | AD7799_CH_AIN2},
    // M7 D23 int temp bipolar gain1, SW=HIGH
    {AD7799_CH2, 1, AD7799_CONF_BIPOLAR | AD7799_CONF_BUF | AD7799_CONF_GAIN(0) | AD7799_CH_INT_TEMP},
};

volatile MFSM_State_t g_fsm_state = MFSM_IDLE;
volatile uint8_t      g_fsm_step  = 0;
volatile uint32_t     g_fsm_codes[7] = {0};
volatile uint8_t      g_fsm_err = 0;
static uint32_t step_t0 = 0;
static uint8_t  running = 0;

void MeasureFSM_Init(void) {
    g_fsm_state = MFSM_IDLE;
    g_fsm_step = 0;
    g_fsm_err = 0;
    running = 0;
    BSP_SW_ADC_Set(0); // безопасное состояние (ТЗ п.2.2.2)
}

void MeasureFSM_Start(void) {
    if (g_fsm_state == MFSM_IDLE) {
        g_fsm_step = 0;
        g_fsm_err = 0;
        running = 1;
    }
}

void MeasureFSM_Task(void) {
    if (!running) return;

    const MFSM_Step_t *s = &steps[g_fsm_step];

    switch (g_fsm_state) {
    case MFSM_IDLE:
        // вход в шаг: эксклюзивность PSW при переходе D22->D23 (ТЗ п.3)
        if (g_fsm_step == 4) {
            AD7799_PowerOff(AD7799_CH1);          // PSW_1 OFF
            step_t0 = GetTick();
            while ((uint32_t)(GetTick() - step_t0) < 10); // пауза 10 мс
        }
        BSP_SW_ADC_Set(s->sw_adc);
        // старт конверсии (Config + Mode + settle)
        AD7799_StartConversion(s->ch, s->config, MFSM_SETTLE_MS);
        step_t0 = GetTick();
        g_fsm_state = MFSM_WAIT_RDY;
        break;

    case MFSM_WAIT_RDY:
        if (AD7799_WaitRDY(s->ch, MFSM_RDY_TMO)) {
            g_fsm_state = MFSM_READ;
        } else {
            g_fsm_err |= (1 << g_fsm_step); // таймаут RDY
            g_fsm_state = MFSM_NEXT;
        }
        break;

    case MFSM_READ:
        g_fsm_codes[g_fsm_step] = AD7799_ReadData(s->ch);
        g_fsm_state = MFSM_NEXT;
        break;

    case MFSM_NEXT:
        if (g_fsm_step < 6) {
            g_fsm_step++;
            g_fsm_state = MFSM_IDLE;
        } else {
            // завершение цикла: PSW_2 OFF
            AD7799_PowerOff(AD7799_CH2);
            g_fsm_state = MFSM_DONE;
            running = 0;
        }
        break;

    case MFSM_DONE:
        g_fsm_state = MFSM_IDLE;
        break;

    default:
        g_fsm_state = MFSM_IDLE;
        break;
    }
}
