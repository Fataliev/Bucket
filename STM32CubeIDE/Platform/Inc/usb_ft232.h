#ifndef USB_FT232_H
#define USB_FT232_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

// Состояние USB подключения
typedef enum {
    USB_DISCONNECTED = 0,
    USB_CONNECTED
} USB_State_t;

// Инициализация USB интерфейса
void USB_FT232_Init(void);

// Отправка данных через USB
uint16_t USB_FT232_Transmit(const uint8_t *data, uint16_t len);

// Прием данных из USB (неблокирующий)
uint16_t USB_FT232_Receive(uint8_t *data, uint16_t max_len);

// Проверка подключения USB (по состоянию DTR#)
USB_State_t USB_FT232_GetState(void);

// Сброс FT232RL
void USB_FT232_Reset(void);

// Отправка строки (удобно для отладки)
void USB_FT232_SendString(const char *str);

// Форматированная отправка (как printf)
void USB_FT232_Printf(const char *format, ...);

void USB_FT232_IRQHandler(void);

#endif // USB_FT232_H
