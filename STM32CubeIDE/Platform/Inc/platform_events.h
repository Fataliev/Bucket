#ifndef PLATFORM_EVENTS_H
#define PLATFORM_EVENTS_H

#include <stdint.h>

/* События планировщика.
   Устанавливаются ТОЛЬКО из прерываний, сбрасываются ТОЛЬКО в SuperCycle_Run(). */
typedef enum {
    PLATFORM_EV_100MS = 0,   /* SysTick : Task_LED                       */
    PLATFORM_EV_1HZ,         /* TIM3    : Task_Heater, Task_Status       */
    PLATFORM_EV_CYCLE_6S,    /* TIM4    : старт измерительного цикла     */
    PLATFORM_EV_USB_RX,      /* USART1  : принят байт от ПК              */
    PLATFORM_EV_UART3_IDLE,  /* USART3  : конец посылки SDI-12 / RS-485  */
    PLATFORM_EV_COUNT
} PlatformEvent_t;

void     Events_Init(void);
void     Events_Set(PlatformEvent_t ev);            /* из ISR                       */
uint8_t  Events_Consume(PlatformEvent_t ev);        /* прочитать и сбросить, из main*/
uint8_t  Events_IsPending(PlatformEvent_t ev);      /* прочитать без сброса         */

/* Метка старта цикла: FSM считает фазы от неё, а не от момента вызова планировщиком */
void     Events_MarkCycleStart(void);               /* из ISR TIM4                  */
uint32_t Events_GetCycleStartTick(void);

#endif /* PLATFORM_EVENTS_H */
