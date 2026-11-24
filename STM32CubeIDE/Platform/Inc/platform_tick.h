#ifndef PLATFORM_TICK_H
#define PLATFORM_TICK_H

#include <stdint.h>

// Инициализация (если потребуется, сейчас пустая)
void Platform_TickInit(void);

// Инкремент тика (вызывается из SysTick_Handler)
void Platform_IncTick(void);

// Получение текущего тика в миллисекундах
uint32_t GetTick(void);

/* Аппаратная задержка по счётчику тактов DWT.
   Работает при запрещённых прерываниях и до запуска SysTick. */
void Platform_DelayUs(uint32_t us);

/* Самопроверка: 1 = системный тик реально инкрементируется */
uint8_t Platform_TickIsRunning(void);
#endif // PLATFORM_TICK_H
