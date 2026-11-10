#include "bsp_gpio.h"

// --- Управление светодиодами ---
void BSP_LED_V15_Set(uint8_t state) {
    if (state) LL_GPIO_SetOutputPin(USB_LED_GPIO_Port, USB_LED_Pin);
    else       LL_GPIO_ResetOutputPin(USB_LED_GPIO_Port, USB_LED_Pin);
}

void BSP_LED_V16_Set(uint8_t state) {
    if (state) LL_GPIO_SetOutputPin(Led_MK_GPIO_Port, Led_MK_Pin);
    else       LL_GPIO_ResetOutputPin(Led_MK_GPIO_Port, Led_MK_Pin);
}

void BSP_LED_V16_Toggle(void) {
    LL_GPIO_TogglePin(Led_MK_GPIO_Port, Led_MK_Pin);
}

// --- Управление CS (Активный уровень - низкий) ---
void BSP_CS_ADC1_Set(uint8_t state) {
    if (state == 0) LL_GPIO_ResetOutputPin(CS_ADC_1_GPIO_Port, CS_ADC_1_Pin); // Активен
    else            LL_GPIO_SetOutputPin(CS_ADC_1_GPIO_Port, CS_ADC_1_Pin);
}

void BSP_CS_ADC2_Set(uint8_t state) {
    if (state == 0) LL_GPIO_ResetOutputPin(CS_ADC_2_GPIO_Port, CS_ADC_2_Pin);
    else            LL_GPIO_SetOutputPin(CS_ADC_2_GPIO_Port, CS_ADC_2_Pin);
}

void BSP_CS_FLASH_Set(uint8_t state) {
    if (state == 0) LL_GPIO_ResetOutputPin(CS_FLASH_GPIO_Port, CS_FLASH_Pin);
    else            LL_GPIO_SetOutputPin(CS_FLASH_GPIO_Port, CS_FLASH_Pin);
}

void BSP_CS_PS09_Set(uint8_t state) {
    if (state == 0) LL_GPIO_ResetOutputPin(CS_PS09_GPIO_Port, CS_PS09_Pin);
    else            LL_GPIO_SetOutputPin(CS_PS09_GPIO_Port, CS_PS09_Pin);
}

// --- PSW Заглушки (в реальности управляются через SPI AD7799) ---
void BSP_PSW_1_Set(uint8_t state) {
    // Пока оставим пустым, так как PSW управляется регистром IO AD7799
    (void)state;
}

void BSP_PSW_2_Set(uint8_t state) {
    (void)state;
}
