#ifndef ONEWIRE_UART_H
#define ONEWIRE_UART_H

#include <stdint.h>

// Инициализация
void OW_Init(void);

// Сброс шины (Reset Pulse). Возвращает 1, если датчик присутствует (Presence Pulse), иначе 0.
uint8_t OW_Reset(void);

// Отправка байта
void OW_WriteByte(uint8_t byte);

// Чтение байта
uint8_t OW_ReadByte(void);

// Отправка и чтение бита (для поиска ROM)
uint8_t OW_ReadBit(void);

#endif // ONEWIRE_UART_H
