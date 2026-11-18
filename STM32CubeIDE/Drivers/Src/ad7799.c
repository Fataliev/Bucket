#include "ad7799.h"
#include "periph_spi.h"
#include "bsp_gpio.h"
#include "platform_tick.h"
#include <stddef.h>

volatile uint8_t g_adc1_type = 0xFF;
volatile uint8_t g_adc2_type = 0xFF;

static void AD7799_SelectCS(AD7799_Channel_t ch, uint8_t state) {
    if (ch == AD7799_CH1) BSP_CS_ADC1_Set(state);
    else BSP_CS_ADC2_Set(state);
}

static void AD7799_SPI_Transfer(AD7799_Channel_t ch, uint8_t *tx, uint8_t *rx, uint16_t len) {
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, len);
    AD7799_SelectCS(ch, 1);
}

void AD7799_Reset(AD7799_Channel_t ch) {
    uint8_t reset_seq[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(reset_seq, NULL, 4);
    AD7799_SelectCS(ch, 1);

    uint32_t t0 = GetTick();
    while ((uint32_t)(GetTick() - t0) < 500);
}

void AD7799_WriteMode(AD7799_Channel_t ch, uint16_t mode_value) {
    uint8_t tx[3] = {AD7799_CMD_WRITE_MODE, (mode_value >> 8) & 0xFF, mode_value & 0xFF};
    uint8_t rx[3] = {0};
    AD7799_SPI_Transfer(ch, tx, rx, 3);
}

void AD7799_WriteConfig(AD7799_Channel_t ch, uint16_t config_value) {
    uint8_t tx[3] = {AD7799_CMD_WRITE_CONFIG, (config_value >> 8) & 0xFF, config_value & 0xFF};
    uint8_t rx[3] = {0};
    AD7799_SPI_Transfer(ch, tx, rx, 3);
}

uint8_t AD7799_ReadStatus(AD7799_Channel_t ch) {
    uint8_t tx[2] = {AD7799_CMD_READ_STATUS, 0x00};
    uint8_t rx[2] = {0};
    AD7799_SPI_Transfer(ch, tx, rx, 2);
    return rx[1];
}

uint16_t AD7799_ReadMode(AD7799_Channel_t ch) {
    uint8_t tx[3] = {AD7799_CMD_READ_MODE, 0x00, 0x00};
    uint8_t rx[3] = {0};
    AD7799_SPI_Transfer(ch, tx, rx, 3);
    return ((uint16_t)rx[1] << 8) | rx[2];
}

uint16_t AD7799_ReadConfig(AD7799_Channel_t ch) {
    uint8_t tx[3] = {AD7799_CMD_READ_CONFIG, 0x00, 0x00};
    uint8_t rx[3] = {0};
    AD7799_SPI_Transfer(ch, tx, rx, 3);
    return ((uint16_t)rx[1] << 8) | rx[2];
}

uint8_t AD7799_ReadIO(AD7799_Channel_t ch) {
    uint8_t tx[2] = {AD7799_CMD_READ_IO, 0x00};
    uint8_t rx[2] = {0};
    AD7799_SPI_Transfer(ch, tx, rx, 2);
    return rx[1];
}

uint32_t AD7799_ReadData(AD7799_Channel_t ch) {
    uint8_t tx[4] = {AD7799_CMD_READ_DATA, 0x00, 0x00, 0x00};
    uint8_t rx[4] = {0};
    AD7799_SPI_Transfer(ch, tx, rx, 4);
    return ((uint32_t)rx[1] << 16) | ((uint32_t)rx[2] << 8) | rx[3];
}

uint8_t AD7799_CheckID(AD7799_Channel_t ch) {
    uint8_t tx[2] = {AD7799_CMD_READ_ID, 0x00};
    uint8_t rx[2] = {0};
    AD7799_SPI_Transfer(ch, tx, rx, 2);
    return rx[1];
}

// Чтение канала: устанавливает Config, запускает Single Conversion, ждёт RDY, читает Data
uint32_t AD7799_ReadChannel(AD7799_Channel_t ch, uint16_t channel_config) {
    // 1. Устанавливаем канал и усиление
    AD7799_WriteConfig(ch, channel_config);

    // 2. Запускаем одиночную конверсию с PSW ON
    AD7799_WriteMode(ch, AD7799_MODE_START);

    // 3. Ждём готовности (до 600 мс для 4.17 Hz)
    if (!AD7799_WaitRDY(ch, 600)) {
        return 0xFFFFFFFF;  // Таймаут
    }

    // 4. Читаем результат
    return AD7799_ReadData(ch);
}

void AD7799_Init(void) {
    AD7799_Reset(AD7799_CH1);
    AD7799_Reset(AD7799_CH2);

    g_adc1_type = AD7799_CheckID(AD7799_CH1);
    g_adc2_type = AD7799_CheckID(AD7799_CH2);

    AD7799_WriteMode(AD7799_CH1, AD7799_MODE_IDLE_OFF);
    AD7799_WriteMode(AD7799_CH2, AD7799_MODE_IDLE_OFF);

    uint32_t t0 = GetTick();
    while ((uint32_t)(GetTick() - t0) < 10);
}

uint8_t AD7799_WaitRDY(AD7799_Channel_t ch, uint32_t timeout_ms) {
    uint32_t t0 = GetTick();
    AD7799_SelectCS(ch, 0);
    while (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_6)) {
        if ((uint32_t)(GetTick() - t0) >= timeout_ms) {
            AD7799_SelectCS(ch, 1);
            return 0;
        }
    }
    AD7799_SelectCS(ch, 1);
    return 1;
}
