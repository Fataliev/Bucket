#include "usb_ft232.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Буфер приема (кольцевой)
#define USB_RX_BUFFER_SIZE 256
static volatile uint8_t rx_buffer[USB_RX_BUFFER_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;

// Инициализация USB FT232RL
void USB_FT232_Init(void) {
    // Настраиваем RESET_USB (PA15) как выход, высокий уровень (не сбрасывать)
    LL_GPIO_SetPinMode(RESET_USB_GPIO_Port, RESET_USB_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetOutputPin(RESET_USB_GPIO_Port, RESET_USB_Pin);

    // Настраиваем DTR# (PA11) и RTS# (PA12) как входы с pull-up
    // FT232RL имеет внутренние pull-up 200кОм, но добавим внешние для надежности
    LL_GPIO_SetPinMode(USART1_DTR_USB_DM_GPIO_Port, USART1_DTR_USB_DM_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(USART1_DTR_USB_DM_GPIO_Port, USART1_DTR_USB_DM_Pin, LL_GPIO_PULL_UP);

    LL_GPIO_SetPinMode(USART1_RTR_USB_DP_GPIO_Port, USART1_RTR_USB_DP_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(USART1_RTR_USB_DP_GPIO_Port, USART1_RTR_USB_DP_Pin, LL_GPIO_PULL_UP);

    // USART1 уже настроен в CubeMX (115200, 8N1)
    // Включаем приемник и передатчик
    LL_USART_Enable(USART1);
    LL_USART_EnableDirectionTx(USART1);
    LL_USART_EnableDirectionRx(USART1);

    // Включаем прерывание по приему (RXNE)
    LL_USART_EnableIT_RXNE(USART1);
}

// Отправка данных через USB
uint16_t USB_FT232_Transmit(const uint8_t *data, uint16_t len) {
    uint16_t sent = 0;

    for (uint16_t i = 0; i < len; i++) {
        // Ждем готовности передатчика
        while (!LL_USART_IsActiveFlag_TXE(USART1));

        // Отправляем байт
        LL_USART_TransmitData8(USART1, data[i]);
        sent++;
    }

    // Ждем завершения передачи последнего байта
    while (!LL_USART_IsActiveFlag_TC(USART1));

    return sent;
}

// Прием данных из USB (неблокирующий)
uint16_t USB_FT232_Receive(uint8_t *data, uint16_t max_len) {
    uint16_t received = 0;

    while (rx_head != rx_tail && received < max_len) {
        data[received++] = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1) % USB_RX_BUFFER_SIZE;
    }

    return received;
}

// Проверка подключения USB (по состоянию DTR#)
// DTR# активный низкий: LOW = подключен, HIGH = отключен
USB_State_t USB_FT232_GetState(void) {
    if (LL_GPIO_IsInputPinSet(USART1_DTR_USB_DM_GPIO_Port, USART1_DTR_USB_DM_Pin)) {
        return USB_DISCONNECTED;  // DTR# = HIGH
    } else {
        return USB_CONNECTED;     // DTR# = LOW
    }
}

// Сброс FT232RL
void USB_FT232_Reset(void) {
    // Активируем сброс (низкий уровень)
    LL_GPIO_ResetOutputPin(RESET_USB_GPIO_Port, RESET_USB_Pin);

    // Ждем 10 мс
    for (volatile uint32_t i = 0; i < 72000 * 10; i++);

    // Снимаем сброс (высокий уровень)
    LL_GPIO_SetOutputPin(RESET_USB_GPIO_Port, RESET_USB_Pin);

    // Ждем инициализации FT232RL (100 мс)
    for (volatile uint32_t i = 0; i < 72000 * 100; i++);
}

// Отправка строки
void USB_FT232_SendString(const char *str) {
    if (str == NULL) return;
    USB_FT232_Transmit((const uint8_t *)str, strlen(str));
}

// Форматированная отправка (как printf)
void USB_FT232_Printf(const char *format, ...) {
    char buffer[256];
    va_list args;

    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    USB_FT232_SendString(buffer);
}

// Обработчик прерывания USART1 (вызывается из stm32f1xx_it.c)
void USB_FT232_IRQHandler(void) {
    // Проверяем флаг приема
    if (LL_USART_IsActiveFlag_RXNE(USART1)) {
        uint8_t data = LL_USART_ReceiveData8(USART1);

        // Сохраняем в кольцевой буфер
        uint16_t next_head = (rx_head + 1) % USB_RX_BUFFER_SIZE;
        if (next_head != rx_tail) {  // Буфер не полон
            rx_buffer[rx_head] = data;
            rx_head = next_head;
        }
    }

    // Очищаем флаг ошибки (если есть)
    if (LL_USART_IsActiveFlag_ORE(USART1)) {
        LL_USART_ReceiveData8(USART1);  // Читаем для сброса флага
    }
}
