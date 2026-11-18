#include "ad7799.h"
#include "periph_spi.h"
#include "bsp_gpio.h"
#include "platform_tick.h"
#include <stddef.h>

static void AD7799_SelectCS(AD7799_Channel_t ch, uint8_t state) {
    if (ch == AD7799_CH1) BSP_CS_ADC1_Set(state);
    else                  BSP_CS_ADC2_Set(state);
}

void AD7799_Reset(AD7799_Channel_t ch) {
    uint8_t seq[4] = {0xFF, 0xFF, 0xFF, 0xFF};   // 32×1 = сброс интерфейса (Rev.B)
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(seq, NULL, 4);
    AD7799_SelectCS(ch, 1);
    uint32_t t0 = GetTick();
    while ((uint32_t)(GetTick() - t0) < 1);        // 500 мкс достаточно, держим 1 мс
}

uint8_t AD7799_ReadStatus(AD7799_Channel_t ch) {
    uint8_t tx[2] = {AD7799_CMD_RD_STATUS, 0x00}, rx[2] = {0,0};
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 2);
    AD7799_SelectCS(ch, 1);
    return rx[1];
}

// ID читается ВСЕГДА (не зависит от режима) — командой 0x44, БЕЗ ожидания RDY
uint8_t AD7799_CheckID(AD7799_Channel_t ch) {
    uint8_t tx[2] = {AD7799_CMD_RD_ID, 0x00}, rx[2] = {0,0};   // 0x44
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 2);
    AD7799_SelectCS(ch, 1);
    return rx[1];
}

void AD7799_WriteMode(AD7799_Channel_t ch, uint16_t mode) {
    uint8_t tx[3] = {AD7799_CMD_WR_MODE, (uint8_t)(mode>>8), (uint8_t)mode}, rx[3];
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 3);
    AD7799_SelectCS(ch, 1);
}

void AD7799_WriteConfig(AD7799_Channel_t ch, uint16_t config) {
    uint8_t tx[3] = {AD7799_CMD_WR_CONFIG, (uint8_t)(config>>8), (uint8_t)config}, rx[3];
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 3);
    AD7799_SelectCS(ch, 1);
}

uint16_t AD7799_ReadMode(AD7799_Channel_t ch) {
    uint8_t tx[3] = {AD7799_CMD_RD_MODE,0,0}, rx[3];
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 3);
    AD7799_SelectCS(ch, 1);
    return ((uint16_t)rx[1]<<8) | rx[2];
}

uint16_t AD7799_ReadConfig(AD7799_Channel_t ch) {
    uint8_t tx[3] = {AD7799_CMD_RD_CONFIG,0,0}, rx[3];
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 3);
    AD7799_SelectCS(ch, 1);
    return ((uint16_t)rx[1]<<8) | rx[2];
}

uint8_t AD7799_ReadIO(AD7799_Channel_t ch) {
    uint8_t tx[2] = {AD7799_CMD_RD_IO,0}, rx[2];
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 2);
    AD7799_SelectCS(ch, 1);
    return rx[1];
}

uint32_t AD7799_ReadData(AD7799_Channel_t ch) {
    uint8_t tx[4] = {AD7799_CMD_RD_DATA,0,0,0}, rx[4];
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 4);
    AD7799_SelectCS(ch, 1);
    return ((uint32_t)rx[1]<<16)|((uint32_t)rx[2]<<8)|rx[3];
}

uint32_t AD7799_ReadOffset(AD7799_Channel_t ch) {
    uint8_t tx[4] = {AD7799_CMD_RD_OFFSET,0,0,0}, rx[4];
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 4);
    AD7799_SelectCS(ch, 1);
    return ((uint32_t)rx[1]<<16)|((uint32_t)rx[2]<<8)|rx[3];
}

uint32_t AD7799_ReadFullScale(AD7799_Channel_t ch) {
    uint8_t tx[4] = {AD7799_CMD_RD_FS,0,0,0}, rx[4];
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 4);
    AD7799_SelectCS(ch, 1);
    return ((uint32_t)rx[1]<<16)|((uint32_t)rx[2]<<8)|rx[3];
}

// RDY активен НИЗКИМ; CS опускаем, чтобы неактивный АЦП был в high-Z (ТЗ п.5)
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

void AD7799_StartConversion(AD7799_Channel_t ch, uint16_t config, uint8_t settle_ms) {
    AD7799_WriteConfig(ch, config);
    AD7799_WriteMode(ch, AD7799_MODE_START);      // single + PSW ON + 4.17 Гц
    uint32_t t0 = GetTick();
    while ((uint32_t)(GetTick() - t0) < settle_ms);
}

void AD7799_PowerOff(AD7799_Channel_t ch) {
    AD7799_WriteMode(ch, AD7799_MODE_IDLE_OFF);   // idle + PSW OFF
}

AD7799_CalResult_t AD7799_CalibrateZeroScale(AD7799_Channel_t ch, uint16_t config) {
    AD7799_WriteConfig(ch, config);
    AD7799_WriteMode(ch, AD7799_MODE_CALZ_ON);    // 0x900F
    if (!AD7799_WaitRDY(ch, 1200)) return AD7799_CAL_TIMEOUT;
    uint8_t st = AD7799_ReadStatus(ch);
    if (st & 0x20) return AD7799_CAL_NO_REF;
    if (st & 0x40) return AD7799_CAL_ERR;
    return AD7799_CAL_OK;
}

void AD7799_Init(void) {
    AD7799_Reset(AD7799_CH1);
    AD7799_Reset(AD7799_CH2);
    uint32_t t0 = GetTick();
    while ((uint32_t)(GetTick() - t0) < 500);     // первая конверсия после сброса
    AD7799_PowerOff(AD7799_CH1);                  // правильный Idle (0x400F), НЕ калибровка
    AD7799_PowerOff(AD7799_CH2);
}
