#ifndef ADG888_H
#define ADG888_H

#include "main.h"

// Состояния ключей ADG888
typedef enum {
    ADG888_POLARITY_FORWARD = 0,  // LOW: D22 активен (прямая полярность)
    ADG888_POLARITY_REVERSE = 1   // HIGH: D23 активен (обратная полярность)
} ADG888_Polarity_t;

// Инициализация драйвера (установка безопасного состояния)
void ADG888_Init(void);

// Установка полярности
void ADG888_SetPolarity(ADG888_Polarity_t polarity);

// Получение текущей полярности
ADG888_Polarity_t ADG888_GetPolarity(void);

// Быстрые inline-функции для критичных по времени участков
static inline void ADG888_Forward(void) {
    ADG888_SetPolarity(ADG888_POLARITY_FORWARD);
}

static inline void ADG888_Reverse(void) {
    ADG888_SetPolarity(ADG888_POLARITY_REVERSE);
}

#endif // ADG888_H
