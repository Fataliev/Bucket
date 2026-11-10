#ifndef PLATFORM_TICK_H
#define PLATFORM_TICK_H

#include <stdint.h>

// Инициализация (если потребуется, сейчас пустая)
void Platform_TickInit(void);

// Инкремент тика (вызывается из SysTick_Handler)
void Platform_IncTick(void);

// Получение текущего тика в миллисекундах
uint32_t GetTick(void);

#endif // PLATFORM_TICK_H
