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

#endif // BSP_GPIO_H
