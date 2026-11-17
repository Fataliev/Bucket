#include "bsp_gpio.h"

// --- Управление светодиодами ---
// ВАЖНО: Судя по схеме (V16), анод светодиода подключен к 3.3В,
// а катод через резистор к PB15. Поэтому для включения (ON) нам нужно
// подать НИЗКИЙ уровень (Reset), а для выключения (OFF) - ВЫСОКИЙ (Set).

void BSP_LED_V16_Set(uint8_t state) {
    if (state) {
    	LL_GPIO_SetOutputPin(Led_MK_GPIO_Port, Led_MK_Pin);   // 1 = Светодиод Горит
    } else {
    	LL_GPIO_ResetOutputPin(Led_MK_GPIO_Port, Led_MK_Pin); // 0 = Светодиод Погашен
    }
}

void BSP_LED_V16_Toggle(void) {
    if (LL_GPIO_IsOutputPinSet(Led_MK_GPIO_Port, Led_MK_Pin)) {
        LL_GPIO_ResetOutputPin(Led_MK_GPIO_Port, Led_MK_Pin); // Был 1 -> ставим 0 (ВКЛ)
    } else {
        LL_GPIO_SetOutputPin(Led_MK_GPIO_Port, Led_MK_Pin);   // Был 0 -> ставим 1 (ВЫКЛ)
    }
}

void BSP_LED_V15_Set(uint8_t state) {
    // На схеме V15 подключен к PB12. Логика аналогичная.
    if (state) LL_GPIO_ResetOutputPin(USB_LED_GPIO_Port, USB_LED_Pin);
    else       LL_GPIO_SetOutputPin(USB_LED_GPIO_Port, USB_LED_Pin);
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
