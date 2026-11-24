#include "platform_tick.h"
#include "main.h"

static volatile uint32_t g_tick_ms = 0U;

void Platform_TickInit(void)
{
    g_tick_ms = 0U;

    /* DWT CYCCNT — счётчик тактов ядра. Нужен для коротких задержек,
       которые не должны зависеть от исправности прерываний. */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
}

void Platform_IncTick(void)
{
    g_tick_ms++;
}

uint32_t GetTick(void)
{
    return g_tick_ms;
}

void Platform_DelayUs(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);

    while ((uint32_t)(DWT->CYCCNT - start) < ticks) {
        /* ожидание по аппаратному счётчику */
    }
}

uint8_t Platform_TickIsRunning(void)
{
    uint32_t t0 = g_tick_ms;

    Platform_DelayUs(3000U);            /* 3 мс, независимо от SysTick */
    return (g_tick_ms != t0) ? 1U : 0U;
}
