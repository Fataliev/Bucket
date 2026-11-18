#ifndef MEASURE_FSM_H
#define MEASURE_FSM_H

#include "main.h"
#include <stdint.h>

// Состояния конечного автомата
typedef enum {
    MEASURE_STATE_IDLE = 0,
    MEASURE_STATE_M1,   // D22 Internal Temp
    MEASURE_STATE_M2,   // D22 B8
    MEASURE_STATE_M3,   // D22 B6
    MEASURE_STATE_M4,   // D22 AIN1 (W_fwd)
    MEASURE_STATE_SWITCH, // Переключение на D23
    MEASURE_STATE_M5,   // D23 AIN1 (W_rev)
    MEASURE_STATE_M6,   // D23 B7
    MEASURE_STATE_M7,   // D23 Internal Temp
    MEASURE_STATE_COMPLETE
} MeasureState_t;

// Структура результатов измерения
typedef struct {
    int32_t d22_temp;
    int32_t d22_b8;
    int32_t d22_b6;
    int32_t w_fwd;
    int32_t w_rev;
    int32_t d23_b7;
    int32_t d23_temp;
    uint32_t timestamp;
} MeasureResult_t;

// Прототипы функций
void MeasureFSM_Init(void);
void MeasureFSM_Task(void);
MeasureResult_t MeasureFSM_GetResult(void);
uint8_t MeasureFSM_IsDataReady(void);
void MeasureFSM_Reset(void);

#endif // MEASURE_FSM_H
