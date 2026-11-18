#include "adg888.h"
#include "bsp_gpio.h"

// Статическая переменная для хранения текущего состояния
static ADG888_Polarity_t current_polarity = ADG888_POLARITY_FORWARD;

void ADG888_Init(void) {
    // Безопасное состояние при сбросе: LOW (прямая полярность, D22 активен)
    ADG888_SetPolarity(ADG888_POLARITY_FORWARD);
}

void ADG888_SetPolarity(ADG888_Polarity_t polarity) {
    if (polarity == ADG888_POLARITY_FORWARD) {
        // LOW: D22 подключен к тензодатчику, D23 к AGND
        LL_GPIO_ResetOutputPin(SW_ADC_GPIO_Port, SW_ADC_Pin);
    } else {
        // HIGH: D23 подключен к тензодатчику, D22 к AGND
        LL_GPIO_SetOutputPin(SW_ADC_GPIO_Port, SW_ADC_Pin);
    }
    current_polarity = polarity;
}

ADG888_Polarity_t ADG888_GetPolarity(void) {
    return current_polarity;
}
