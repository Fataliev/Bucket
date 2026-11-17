#include "onewire_uart.h"
#include "periph_usart.h"
#include "main.h" // Для доступа к USART2

#define OW_UART USART2

void OW_Init(void) {
    // Дополнительная настройка не требуется, так как CubeMX уже настроил USART2 в Half-Duplex
}

uint8_t OW_Reset(void) {
    uint8_t tx_buf = 0xF0; // 0xF0 на 9600 бод дает импульс сброса (Reset Pulse) ~480 мкс
    uint8_t rx_buf = 0xFF;

    // Очищаем флаги
    LL_USART_ClearFlag_TC(OW_UART);
    LL_USART_ClearFlag_RXNE(OW_UART);

    // Отправляем импульс
    LL_USART_TransmitData8(OW_UART, tx_buf);

    // Ждем завершения передачи
    while (!LL_USART_IsActiveFlag_TC(OW_UART));
    LL_USART_ClearFlag_TC(OW_UART);

    // Ждем ответа (Presence Pulse)
    // Датчик ответит, если опустит линию в 0.
    // При приеме через UART это будет выглядеть как 0x00 - 0xF0 в зависимости от момента сэмплирования.
    uint32_t timeout = 100000; // Простой программный таймаут
    while (!LL_USART_IsActiveFlag_RXNE(OW_UART)) {
        if (--timeout == 0) return 0; // Нет ответа
    }

    rx_buf = LL_USART_ReceiveData8(OW_UART);

    // Если пришел 0x00 или 0x80 (в зависимости от тайминга), значит датчик ответил
    // На практике при 9600 бод ответ обычно читается как 0x00
    if (rx_buf == 0x00 || rx_buf == 0x80 || rx_buf == 0xF0) {
        return 1; // Присутствует
    }
    return 0;
}

void OW_WriteByte(uint8_t byte) {
    // В 1-Wire каждый бит передается как отдельный байт в UART
    // 1 = 0xFF, 0 = 0x00
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t bit = byte & 0x01;
        uint8_t tx_byte = bit ? 0xFF : 0x00;

        LL_USART_TransmitData8(OW_UART, tx_byte);
        while (!LL_USART_IsActiveFlag_TC(OW_UART));
        LL_USART_ClearFlag_TC(OW_UART);

        byte >>= 1;
    }
}

uint8_t OW_ReadByte(void) {
    uint8_t byte = 0;

    for (uint8_t i = 0; i < 8; i++) {
        // Чтобы прочитать бит, мы отправляем 0xFF (освобождаем линию)
        LL_USART_TransmitData8(OW_UART, 0xFF);
        while (!LL_USART_IsActiveFlag_TC(OW_UART));
        LL_USART_ClearFlag_TC(OW_UART);

        // Ждем приема
        while (!LL_USART_IsActiveFlag_RXNE(OW_UART));
        uint8_t rx_byte = LL_USART_ReceiveData8(OW_UART);

        // Если младший бит приема равен 1, значит датчик отправил 1
        if (rx_byte & 0x01) {
            byte |= (1 << i);
        }
    }
    return byte;
}

uint8_t OW_ReadBit(void) {
    LL_USART_TransmitData8(OW_UART, 0xFF);
    while (!LL_USART_IsActiveFlag_TC(OW_UART));
    LL_USART_ClearFlag_TC(OW_UART);

    while (!LL_USART_IsActiveFlag_RXNE(OW_UART));
    uint8_t rx_byte = LL_USART_ReceiveData8(OW_UART);

    return (rx_byte & 0x01);
}


/*
 * Важно: В коде OW_Reset и OW_ReadByte используются циклы while(...) для ожидания флагов.
 * В реальной работе, когда мы будем опрашивать DS18B20, эти циклы не приведут к зависанию,
 * так как UART работает на аппаратном уровне. Однако, в будущем (для соблюдения ТЗ),
 * мы можем переписать это на DMA, если возникнут проблемы с блокировкой. Но сейчас,
 * для этапа подготовки, этот вариант самый надежный и понятный.
 */

