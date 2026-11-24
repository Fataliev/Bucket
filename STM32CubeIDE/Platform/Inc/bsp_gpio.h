#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "main.h" // Для доступа к LL_GPIO

// Управление светодиодами
void BSP_LED_V15_Set(uint8_t state);
void BSP_LED_V16_Set(uint8_t state);
void BSP_LED_V16_Toggle(void);

// Управление CS линиями
void BSP_CS_ADC1_Set(uint8_t state);
void BSP_CS_ADC2_Set(uint8_t state);
void BSP_CS_FLASH_Set(uint8_t state);
void BSP_CS_PS09_Set(uint8_t state);

// Управление питанием тензодатчика (PSW)
void BSP_PSW_1_Set(uint8_t state);
void BSP_PSW_2_Set(uint8_t state);
void BSP_SW_ADC_Set(uint8_t state); // PB5: 0=D22(прямая), 1=D23(обратная)

/* Объединённый DOUT/~RDY обоих АЦП на SPI1_MISO (PA6).
   Читать только при опущенном CS конкретного АЦП.
   PA6 настроен как Floating Input в MX_SPI1_Init — уровень читается корректно. */
static inline uint8_t BSP_ADC_RDY_Read(void)
{
    return (uint8_t)LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_6);
}

/* 1 = данные готовы (~RDY низкий) */
static inline uint8_t BSP_ADC_RdyIsLow(void)
{
    return (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_6) == 0U) ? 1U : 0U;
}

#endif // BSP_GPIO_H
