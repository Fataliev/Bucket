#include "measure_fsm.h"
#include "ad7799.h"
#include "adg888.h"
#include "platform_tick.h"

#define MEASURE_STEP_DURATION   480
#define SWITCH_DELAY            15
#define SETTLE_DELAY            5

static MeasureState_t current_state = MEASURE_STATE_IDLE;
static MeasureResult_t measure_result;
static uint32_t step_start_time = 0;
static uint8_t data_ready = 0;
static uint8_t current_step = 0;

void MeasureFSM_Init(void) {
    current_state = MEASURE_STATE_IDLE;
    data_ready = 0;
    current_step = 0;
    AD7799_Init();
    ADG888_Init(); // Убедитесь, что ADG888_Init() существует в adg888.c
}

void MeasureFSM_Task(void) {
    uint32_t current_time = GetTick();
    uint32_t elapsed = current_time - step_start_time;

    if (current_state == MEASURE_STATE_IDLE) {
        current_step = 0;
        current_state = MEASURE_STATE_M1;
        step_start_time = current_time;
        AD7799_WriteMode(AD7799_CH1, 0x3000); // PSW ON
        return;
    }

    // Массив конфигураций для шагов
    static const struct {
        AD7799_Channel_t ch;
        uint16_t config;
    } steps[] = {
        {AD7799_CH1, AD7799_CONF_CH_INT_TEMP | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(0)},
        {AD7799_CH1, AD7799_CONF_CH_AIN3 | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(0)},
        {AD7799_CH1, AD7799_CONF_CH_AIN2 | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(0)},
        {AD7799_CH1, AD7799_CONF_CH_AIN1 | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(7)},
        {AD7799_CH2, AD7799_CONF_CH_AIN1 | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(7)},
        {AD7799_CH2, AD7799_CONF_CH_AIN2 | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(0)},
        {AD7799_CH2, AD7799_CONF_CH_INT_TEMP | AD7799_CONF_BUF_ON | AD7799_CONF_GAIN(0)}
    };

    if (current_step < 7) {
        if (elapsed >= MEASURE_STEP_DURATION) {
            // Запуск конверсии
            AD7799_WriteConfig(steps[current_step].ch, steps[current_step].config);
            AD7799_WriteMode(steps[current_step].ch, 0x300F); // Single + PSW ON + 4.17Hz

            if (AD7799_WaitRDY(steps[current_step].ch, 600)) {
                uint32_t raw_data = AD7799_ReadData(steps[current_step].ch);

                switch(current_step) {
                    case 0: measure_result.d22_temp = (int32_t)raw_data; break;
                    case 1: measure_result.d22_b8 = (int32_t)raw_data; break;
                    case 2: measure_result.d22_b6 = (int32_t)raw_data; break;
                    case 3: measure_result.w_fwd = (int32_t)raw_data; break;
                    case 4: measure_result.w_rev = (int32_t)raw_data; break;
                    case 5: measure_result.d23_b7 = (int32_t)raw_data; break;
                    case 6: measure_result.d23_temp = (int32_t)raw_data; break;
                }
            }

            current_step++;
            step_start_time = GetTick();

            // Логика переключения между D22 и D23 (после шага 3, перед шагом 4)
            if (current_step == 4) {
                AD7799_WriteMode(AD7799_CH1, 0x400F); // D22 OFF
                // ADG888_Reverse(); // Раскомментируйте, когда реализуете adg888.c
                AD7799_WriteMode(AD7799_CH2, 0x3000); // D23 PSW ON

                // Settling 5 мс
                uint32_t t_settle = GetTick();
                while ((GetTick() - t_settle) < SETTLE_DELAY);
                step_start_time = GetTick();
            }
        }
    } else {
        // Завершение цикла
        AD7799_WriteMode(AD7799_CH2, 0x400F); // D23 OFF
        // ADG888_Forward(); // Раскомментируйте, когда реализуете adg888.c

        measure_result.timestamp = GetTick();
        data_ready = 1;
        current_state = MEASURE_STATE_COMPLETE;
    }
}

MeasureResult_t MeasureFSM_GetResult(void) {
    return measure_result;
}

uint8_t MeasureFSM_IsDataReady(void) {
    if (data_ready) {
        data_ready = 0;
        return 1;
    }
    return 0;
}

void MeasureFSM_Reset(void) {
    current_state = MEASURE_STATE_IDLE;
    data_ready = 0;
    current_step = 0;
}
