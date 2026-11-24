#ifndef PLATFORM_ISR_H
#define PLATFORM_ISR_H

/* Единая точка входа прерываний в слой Platform.
   Core/stm32f1xx_it.c содержит только вызовы этих функций. */
void Platform_ISR_SysTick(void);      /* 1 мс  */
void Platform_ISR_TIM3(void);         /* 1 Гц  */
void Platform_ISR_TIM4(void);         /* 6 с   */
void Platform_ISR_SPI1(void);
void Platform_ISR_USART1(void);
void Platform_ISR_DMA1_CH2(void);     /* USART3_TX */
void Platform_ISR_DMA1_CH4(void);     /* USART1_TX */
void Platform_ISR_DMA1_CH7(void);     /* USART2_TX */

#endif /* PLATFORM_ISR_H */
