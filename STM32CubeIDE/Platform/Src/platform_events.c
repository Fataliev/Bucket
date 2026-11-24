#include "platform_events.h"
#include "platform_tick.h"
#include "main.h"   /* CMSIS: __get_PRIMASK / __set_PRIMASK / __disable_irq */

static volatile uint8_t  g_flags[PLATFORM_EV_COUNT];
static volatile uint32_t g_cycle_start_tick = 0U;

void Events_Init(void) {
    uint32_t i;
    for (i = 0U; i < (uint32_t)PLATFORM_EV_COUNT; i++) {
        g_flags[i] = 0U;
    }
    g_cycle_start_tick = 0U;
}

/* Вызывается из ISR: запись одного байта атомарна,
   а main в это время либо не в критической секции, либо с запрещёнными IRQ. */
void Events_Set(PlatformEvent_t ev) {
    if (ev < PLATFORM_EV_COUNT) {
        g_flags[ev] = 1U;
    }
}

/* Чтение-со-сбросом обязано быть атомарным: иначе ISR, вклинившийся
   между чтением и сбросом, потеряет событие. */
uint8_t Events_Consume(PlatformEvent_t ev) {
    uint8_t  value = 0U;
    uint32_t primask;

    if (ev < PLATFORM_EV_COUNT) {
        primask = __get_PRIMASK();
        __disable_irq();
        value = g_flags[ev];
        g_flags[ev] = 0U;
        __set_PRIMASK(primask);
    }
    return value;
}

uint8_t Events_IsPending(PlatformEvent_t ev) {
    return (ev < PLATFORM_EV_COUNT) ? g_flags[ev] : 0U;
}

void Events_MarkCycleStart(void) {
    g_cycle_start_tick = GetTick();
}

uint32_t Events_GetCycleStartTick(void) {
    return g_cycle_start_tick;
}
