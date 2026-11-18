#include "ad7799.h"
#include "periph_spi.h"
#include "bsp_gpio.h"
#include "platform_tick.h"
#include <stddef.h>

// Forward declarations
static void AD7799_SelectCS(AD7799_Channel_t ch, uint8_t state);
static void AD7799_SetIdleMode(AD7799_Channel_t ch);

static void AD7799_SelectCS(AD7799_Channel_t ch, uint8_t state) {
    if (ch == AD7799_CH1) {
        BSP_CS_ADC1_Set(state);
    } else {
        BSP_CS_ADC2_Set(state);
    }
}

static void AD7799_SetIdleMode(AD7799_Channel_t ch) {
    // Mode Register: Idle + PSW ON = 0x3000
    uint8_t tx_buf[3] = {0x08, 0x30, 0x00};
    uint8_t rx_buf[3] = {0};
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 3);
    AD7799_SelectCS(ch, 1);
}

void AD7799_Reset(AD7799_Channel_t ch) {
    uint8_t reset_seq[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(reset_seq, NULL, 4);
    AD7799_SelectCS(ch, 1);
}

void AD7799_Init(void) {
    // Принудительный сброс обоих каналов
    AD7799_ForceReset(AD7799_CH1);
    AD7799_ForceReset(AD7799_CH2);

    // Теперь читаем ID
    uint8_t  g_adc1_id = AD7799_CheckID(AD7799_CH1);
    uint8_t  g_adc2_id = AD7799_CheckID(AD7799_CH2);
}

// Прямое чтение ID без промежуточных функций
uint8_t AD7799_CheckID(AD7799_Channel_t ch) {
    uint8_t tx_buf[2] = {0x44, 0x00};  // Чтение ID Register (именно 44, не C0)
    uint8_t rx_buf[2] = {0x00, 0x00};

    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 2);
    AD7799_SelectCS(ch, 1);

    // rx_buf[0] = статус во время приёма команды
    // rx_buf[1] = содержимое ID Register (должно быть 0x0A для AD7799)
    return rx_buf[1];
}

uint32_t AD7799_ReadData(AD7799_Channel_t ch) {
    uint8_t tx_buf[4] = {0x58, 0x00, 0x00, 0x00};
    uint8_t rx_buf[4] = {0};

    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 4);
    AD7799_SelectCS(ch, 1);

    return ((uint32_t)rx_buf[1] << 16) | ((uint32_t)rx_buf[2] << 8) | rx_buf[3];
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


void AD7799_ForceReset(AD7799_Channel_t ch) {
    // 1. Полный сброс: 32 бита 0xFF
    uint8_t reset_seq[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(reset_seq, NULL, 4);
    AD7799_SelectCS(ch, 1);

    // 2. Ждем 500 мс (время завершения конверсии после сброса)
    uint32_t t0 = GetTick();
    while ((uint32_t)(GetTick() - t0) < 500);

    // 3. Принудительно переводим в Idle mode
    // Mode Register: MD=010 (Idle), PSW=0, FS=0000 → 0x2000
    uint8_t tx_buf[3] = {0x08, 0x20, 0x00};
    uint8_t rx_buf[3] = {0};
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 3);
    AD7799_SelectCS(ch, 1);

    // 4. Ждем 10 мс
    t0 = GetTick();
    while ((uint32_t)(GetTick() - t0) < 10);

    // 5. Проверяем Mode Register (должен быть 0x2000)
    tx_buf[0] = 0x48;  // Чтение Mode Register
    tx_buf[1] = 0x00;
    tx_buf[2] = 0x00;
    AD7799_SelectCS(ch, 0);
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 3);
    AD7799_SelectCS(ch, 1);

    // rx_buf[1] = Mode High byte (должен быть 0x20)
    // rx_buf[2] = Mode Low byte (должен быть 0x00)
}




