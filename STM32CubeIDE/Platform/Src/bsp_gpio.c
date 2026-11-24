#include "bsp_gpio.h"

// --- Управление светодиодами ---
// ВАЖНО: Судя по схеме (V16), анод светодиода подключен через резистор R65=1.8kOm к PB15,
// а катод 0V. Поэтому для выключения (OFF)  нужно подать НИЗКИЙ уровень (Reset)
// а для включения (ON) - ВЫСОКИЙ (Set).

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
    /* PB12 -> R57 390 Ом -> анод V15, катод GND: высокий = горит если подключен USB */
    if (state) LL_GPIO_SetOutputPin(USB_LED_GPIO_Port,   USB_LED_Pin);
    else       LL_GPIO_ResetOutputPin(USB_LED_GPIO_Port, USB_LED_Pin);
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

void BSP_SW_ADC_Set(uint8_t state) {
    if (state) LL_GPIO_SetOutputPin(SW_ADC_GPIO_Port, SW_ADC_Pin);
    else       LL_GPIO_ResetOutputPin(SW_ADC_GPIO_Port, SW_ADC_Pin);
}

/* Диагностика: активен ли CS (низкий уровень) */
static inline uint8_t BSP_CS_ADC1_IsActive(void) {
    return (LL_GPIO_IsInputPinSet(CS_ADC_1_GPIO_Port, CS_ADC_1_Pin) == 0U) ? 1U : 0U;
}
static inline uint8_t BSP_CS_ADC2_IsActive(void) {
    return (LL_GPIO_IsInputPinSet(CS_ADC_2_GPIO_Port, CS_ADC_2_Pin) == 0U) ? 1U : 0U;
}

/* Объединённый DOUT/~RDY обоих АЦП на SPI1_MISO (PA6).
   Читать только при опущенном CS конкретного АЦП.
   PA6 настроен как Floating Input в MX_SPI1_Init — уровень читается корректно. */

