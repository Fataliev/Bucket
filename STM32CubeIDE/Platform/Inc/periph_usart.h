#ifndef PERIPH_USART_H
#define PERIPH_USART_H

#include <stdint.h>
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_dma.h"

// Идентификаторы USART для удобства
typedef enum {
    UART_PORT_1 = 1, // USB (FT232)
    UART_PORT_2 = 2, // 1-Wire
    UART_PORT_3 = 3  // RS-485 / SDI-12
} UART_Port_t;

// Инициализация оберток
void USART_Platform_Init(void);

// Блокирующая отправка (для отладки или коротких команд)
void USART_Transmit_Blocking(USART_TypeDef *USARTx, uint8_t *data, uint16_t len);

// Асинхронная отправка через DMA
// Возвращает 1, если отправка запущена, 0 - если занято
uint8_t USART_Transmit_DMA(USART_TypeDef *USARTx, uint8_t *data, uint16_t len);

// Проверка, занят ли передатчик DMA
uint8_t USART_IsTxBusy(USART_TypeDef *USARTx);

// Включение прерывания по IDLE (для приема команд SDI-12/RS-485)
void USART_EnableIdleIT(USART_TypeDef *USARTx);

#endif // PERIPH_USART_H
