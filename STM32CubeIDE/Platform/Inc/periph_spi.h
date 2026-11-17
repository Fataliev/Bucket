#ifndef PERIPH_SPI_H
#define PERIPH_SPI_H

#include <stdint.h>
#include "stm32f1xx_ll_spi.h"

// Прототип функции-коллбэка.
// context - указатель на контекст (например, хэндл конкретного АЦП)
// data - указатель на буфер с принятыми данными
// len - количество принятых байт
typedef void (*SPI_Callback_t)(void *context, uint8_t *data, uint16_t len);

// Инициализация модуля (сброс состояний)
void SPI_Platform_Init(void);

// Блокирующая передача (используется для команд инициализации)
void SPI_TransmitReceive_Blocking(uint8_t *txData, uint8_t *rxData, uint16_t len);

// Асинхронная передача/прием через прерывания
// Возвращает 1 при успешном запуске, 0 если шина занята
uint8_t SPI_TransmitReceive_IT(uint8_t *txData, uint8_t *rxData, uint16_t len,
                               SPI_Callback_t callback, void *context);

// Проверка, занята ли шина в данный момент
uint8_t SPI_IsBusy(void);

// Эта функция вызывается из stm32f1xx_it.c (SPI1_IRQHandler)
void SPI_IRQ_Handler(void);

#endif // PERIPH_SPI_H
