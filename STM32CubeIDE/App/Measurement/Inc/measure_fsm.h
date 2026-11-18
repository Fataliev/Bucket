#ifndef MEASURE_FSM_H
#define MEASURE_FSM_H

#include <stdint.h>
#include "ad7799.h"

typedef enum {
    MFSM_IDLE = 0,
    MFSM_SETTLE,    // ждём settle после старта
    MFSM_WAIT_RDY,  // ждём RDY low
    MFSM_READ,      // читаем 24 бита
    MFSM_NEXT,      // переход к следующему шагу
    MFSM_DONE       // цикл завершён
} MFSM_State_t;

typedef struct {
    AD7799_Channel_t ch;     // D22/D23
    uint8_t  sw_adc;         // уровень PB5
    uint16_t config;         // Config Register
} MFSM_Step_t;

void MeasureFSM_Init(void);
void MeasureFSM_Task(void);   // вызывать в main loop
void MeasureFSM_Start(void);  // запустить один super-cycle

extern volatile MFSM_State_t g_fsm_state;
extern volatile uint8_t      g_fsm_step;
extern volatile uint32_t     g_fsm_codes[7]; // сырые коды M1..M7
extern volatile uint8_t      g_fsm_err;      // биты ошибок шага

#endif
