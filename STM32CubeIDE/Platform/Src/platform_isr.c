#include "platform_isr.h"
#include "platform_tick.h"
#include "platform_events.h"
#include "periph_spi.h"
#include "periph_usart.h"
#include "usb_ft232.h"
#include "main.h"

static uint16_t s_div_100ms = 0U;

void Platform_ISR_SysTick(void) {
    Platform_IncTick();

    s_div_100ms++;
    if (s_div_100ms >= 100U) {
        s_div_100ms = 0U;
        Events_Set(PLATFORM_EV_100MS);
    }
}

void Platform_ISR_TIM3(void) {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM3) != RESET) {
        LL_TIM_ClearFlag_UPDATE(TIM3);
        Events_Set(PLATFORM_EV_1HZ);
    }
}

void Platform_ISR_TIM4(void) {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM4) != RESET) {
        LL_TIM_ClearFlag_UPDATE(TIM4);
        Events_MarkCycleStart();          /* фазы FSM считаются от этой метки */
        Events_Set(PLATFORM_EV_CYCLE_6S);
    }
}

void Platform_ISR_SPI1(void)   { SPI_IRQ_Handler(); }
void Platform_ISR_USART1(void) { USB_FT232_IRQHandler(); Events_Set(PLATFORM_EV_USB_RX); }
void Platform_ISR_DMA1_CH2(void) { USART3_TX_DMA_IRQHandler(); }
void Platform_ISR_DMA1_CH4(void) { USART1_TX_DMA_IRQHandler(); }
void Platform_ISR_DMA1_CH7(void) { USART2_TX_DMA_IRQHandler(); }
