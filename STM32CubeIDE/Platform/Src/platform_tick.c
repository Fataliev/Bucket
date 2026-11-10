#include "platform_tick.h"

// Глобальная переменная для хранения тика
static volatile uint32_t g_tick_ms = 0;

void Platform_TickInit(void) {
    g_tick_ms = 0;
}

void Platform_IncTick(void) {
    g_tick_ms++;
}

uint32_t GetTick(void) {
    return g_tick_ms;
}

