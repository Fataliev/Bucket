#ifndef MEASURE_FSM_H
#define MEASURE_FSM_H

#include <stdint.h>

/* Индексы шагов измерительного цикла (ТЗ 2.2.1).
   Шаг 0 и 6 — монитор AVDD, а не температура кристалла (см. ad7799.h). */
typedef enum {
    MEAS_STEP_AVDD_D22 = 0,   /* 0    мс : D22 CH7  */
    MEAS_STEP_B8       = 1,   /* 480  мс : D22 AIN3 */
    MEAS_STEP_B6       = 2,   /* 960  мс : D22 AIN2 */
    MEAS_STEP_W_FWD    = 3,   /* 1440 мс : D22 AIN1 */
    MEAS_STEP_W_REV    = 4,   /* 1920 мс : D23 AIN1 (после переключения ключей) */
    MEAS_STEP_B7       = 5,   /* 2400 мс : D23 AIN2 */
    MEAS_STEP_AVDD_D23 = 6,   /* 2880 мс : D23 CH7  */
    MEAS_STEP_COUNT    = 7
} MeasureStep_t;

/* Результат одного завершённого цикла. Коды сырые, без пересчёта. */
typedef struct {
    int32_t  w_fwd;                       /* D22 AIN1, bipolar, G=128 */
    int32_t  w_rev;                       /* D23 AIN1, bipolar, G=128 */
    int32_t  b6;                          /* D22 AIN2, unipolar, G=1  */
    int32_t  b8;                          /* D22 AIN3, unipolar, G=1  */
    int32_t  b7;                          /* D23 AIN2, unipolar, G=1  */
    int32_t  avdd_d22;                    /* D22 CH7 (AVDD/6)         */
    int32_t  avdd_d23;                    /* D23 CH7 (AVDD/6)         */

    uint8_t  valid_mask;                  /* бит i: шаг i достоверен  */
    uint32_t error_mask;                  /* бит i: таймаут; 8+i: NOREF; 16+i: ERR */

    uint32_t cycle_start;                 /* метка TIM4               */
    uint32_t timestamp;                   /* GetTick() на завершении  */

    uint16_t step_ms[MEAS_STEP_COUNT];    /* фактическое время до ~RDY, диагностика */
    uint8_t  step_status[MEAS_STEP_COUNT];/* регистр статуса после шага */
    uint16_t step_mode[MEAS_STEP_COUNT];  /* Mode после шага: контроль бита PSW */
} MeasureResult_t;

void                   MeasureFSM_Init(void);
void                   MeasureFSM_Start(uint32_t cycle_start_tick);
void                   MeasureFSM_Task(void);          /* неблокирующий, вызов каждый проход */
void                   MeasureFSM_Reset(void);

uint8_t                MeasureFSM_IsActive(void);
uint8_t                MeasureFSM_IsDataReady(void);   /* читает со сбросом */
MeasureResult_t        MeasureFSM_GetResult(void);
const MeasureResult_t *MeasureFSM_GetResultPtr(void);
uint32_t               MeasureFSM_GetCycleCount(void);
uint32_t               MeasureFSM_GetOverrunCount(void);

#endif /* MEASURE_FSM_H */
