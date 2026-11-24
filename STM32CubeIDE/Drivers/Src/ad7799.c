#include "ad7799.h"
#include "periph_spi.h"
#include "bsp_gpio.h"
#include "platform_tick.h"
#include <stddef.h>





static volatile uint16_t s_not_ready_cnt[2] = {0U, 0U};

static void AD7799_SelectCS(AD7799_Device_t dev, uint8_t state) {
    if (dev == AD7799_DEV_1) {
        BSP_CS_ADC1_Set(state);
    } else {
        BSP_CS_ADC2_Set(state);
    }
}

void AD7799_Reset(AD7799_Device_t dev) {
uint8_t reset_seq[4] = {0xFF, 0xFF, 0xFF, 0xFF};
AD7799_SelectCS(dev, 0);
SPI_TransmitReceive_Blocking(reset_seq, NULL, 4);
AD7799_SelectCS(dev, 1);
Platform_DelayUs(500U);   /* ТЗ 2.2.7: 500 мкс, не зависит от SysTick */
}

void AD7799_SetIdleMode(AD7799_Device_t dev) {
    uint8_t tx_buf[3] = {
        AD7799_CMD_WR_MODE,
        (uint8_t)(AD7799_MODE_IDLE_OFF >> 8),
        (uint8_t)AD7799_MODE_IDLE_OFF
    };
    uint8_t rx_buf[3] = {0};
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx_buf, rx_buf, 3);
    AD7799_SelectCS(dev, 1);
}

void AD7799_Init(void) {
AD7799_Reset(AD7799_DEV_1);
AD7799_Reset(AD7799_DEV_2);
AD7799_WriteMode(AD7799_DEV_1, AD7799_MODE_IDLE_OFF);
AD7799_WriteMode(AD7799_DEV_2, AD7799_MODE_IDLE_OFF);
Platform_DelayUs(10000U);  /* 10 мс с запасом; блокировка до старта тика исключена */
}

uint8_t AD7799_ReadStatus(AD7799_Device_t dev) {
    uint8_t tx[2] = {AD7799_CMD_RD_STATUS, 0x00}, rx[2] = {0,0};
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 2);
    AD7799_SelectCS(dev, 1);
    return rx[1];
}

uint8_t AD7799_ReadID(AD7799_Device_t dev) {
    uint8_t tx[2] = {AD7799_CMD_RD_ID, 0x00}, rx[2] = {0,0};
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 2);
    AD7799_SelectCS(dev, 1);
    return rx[1];
}

bool AD7799_CheckID(AD7799_Device_t dev) {
    uint8_t id = AD7799_ReadID(dev);
    return ((id & 0x0F) == AD7799_ID_AD7799);
}

uint16_t AD7799_ReadMode(AD7799_Device_t dev) {
    uint8_t tx[3] = {AD7799_CMD_RD_MODE,0,0}, rx[3];
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 3);
    AD7799_SelectCS(dev, 1);
    return ((uint16_t)rx[1]<<8) | rx[2];
}

uint16_t AD7799_ReadConfig(AD7799_Device_t dev) {
uint8_t tx[3] = {AD7799_CMD_RD_CONFIG, 0, 0};
uint8_t rx[3];
AD7799_SelectCS(dev, 0);
SPI_TransmitReceive_Blocking(tx, rx, 3);
AD7799_SelectCS(dev, 1);
/* ОТЛАДКА УДАЛЕНА: блокирующий printf внутри драйвера ломает тайминги */
return ((uint16_t)rx[1] << 8) | rx[2];
}

uint8_t AD7799_ReadIO(AD7799_Device_t dev) {
    uint8_t tx[2] = {AD7799_CMD_RD_IO,0}, rx[2];
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 2);
    AD7799_SelectCS(dev, 1);
    return rx[1];
}

uint32_t AD7799_ReadData(AD7799_Device_t dev) {
    uint8_t tx[4] = {AD7799_CMD_RD_DATA,0,0,0}, rx[4];
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 4);
    AD7799_SelectCS(dev, 1);
    return ((uint32_t)rx[1]<<16)|((uint32_t)rx[2]<<8)|rx[3];
}

uint32_t AD7799_ReadOffset(AD7799_Device_t dev) {
    uint8_t tx[4] = {AD7799_CMD_RD_OFFSET,0,0,0}, rx[4];
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 4);
    AD7799_SelectCS(dev, 1);
    return ((uint32_t)rx[1]<<16)|((uint32_t)rx[2]<<8)|rx[3];
}

uint32_t AD7799_ReadFullScale(AD7799_Device_t dev) {
    uint8_t tx[4] = {AD7799_CMD_RD_FS,0,0,0}, rx[4];
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 4);
    AD7799_SelectCS(dev, 1);
    return ((uint32_t)rx[1]<<16)|((uint32_t)rx[2]<<8)|rx[3];
}

void AD7799_WriteMode(AD7799_Device_t dev, uint16_t mode) {
    uint8_t tx[3] = {AD7799_CMD_WR_MODE, (uint8_t)(mode>>8), (uint8_t)mode}, rx[3];
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 3);
    AD7799_SelectCS(dev, 1);
}

void AD7799_WriteConfig(AD7799_Device_t dev, uint16_t config) {
    uint8_t tx[3] = {AD7799_CMD_WR_CONFIG, (uint8_t)(config>>8), (uint8_t)config}, rx[3];
    AD7799_SelectCS(dev, 0);
    SPI_TransmitReceive_Blocking(tx, rx, 3);
    AD7799_SelectCS(dev, 1);
}

uint8_t AD7799_WaitRDY(AD7799_Device_t dev, uint32_t timeout_ms) {
    uint32_t t0 = GetTick();
    AD7799_SelectCS(dev, 0);
    while (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_6)) {
        if ((uint32_t)(GetTick() - t0) >= timeout_ms) {
            AD7799_SelectCS(dev, 1);
            return 0;
        }
    }
    AD7799_SelectCS(dev, 1);
    return 1;
}






uint32_t AD7799_MeasureChannel(AD7799_Device_t dev, uint16_t config) {
	 AD7799_WriteMode(dev, AD7799_MODE_IDLE_ON);
	    AD7799_WriteConfig(dev, config);


	    uint32_t t0 = GetTick();
	    while ((uint32_t)(GetTick() - t0) < 50);

	    AD7799_WriteMode(dev, AD7799_MODE_START);
	    uint32_t data = 0xFFFFFFFF;
	    if (AD7799_WaitRDY(dev, 700)) {
	        data = AD7799_ReadData(dev);
	    }
	    AD7799_WriteMode(dev, AD7799_MODE_IDLE_OFF);
	    return data;
}


/* ======================= Неблокирующий интерфейс ======================= */




/* ======================= Неблокирующий интерфейс ======================= */

/* Пауза после падения CS: выход DOUT/~RDY из Z-состояния и перезаряд
   линии через R64 4.7k. ~2 мкс при 72 МГц. */
static void AD7799_CsSettleDelay(void)
{
    uint32_t i;
    for (i = 0U; i < 150U; i++) {
        __NOP();
    }
}

void AD7799_StartConversion(AD7799_Device_t dev, uint16_t config, uint8_t psw_on)
{
    uint16_t mode = AD7799_MODE_SINGLE | AD7799_FS_417HZ;

    if (psw_on != 0U) {
        mode |= AD7799_MODE_PSW_ON;
    }
    AD7799_WriteConfig(dev, config);
    AD7799_WriteMode(dev, mode);
}

uint8_t AD7799_IsReady(AD7799_Device_t dev)
{
	   // Пин PA6 при CS=1 работает как ~RDY (открытый сток).
	    // Низкий уровень = данные готовы.
	    return LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_6) ? 0 : 1;
}

uint8_t AD7799_ReadResult(AD7799_Device_t dev, uint32_t *code, uint8_t *status)
{
    uint32_t raw = AD7799_ReadData(dev);
    uint8_t  st  = AD7799_ReadStatus(dev);

    if (code   != NULL) { *code   = raw; }
    if (status != NULL) { *status = st;  }

    return ((st & (AD7799_ST_ERR | AD7799_ST_NOREF)) == 0U) ? 1U : 0U;
}

void AD7799_SetPSW(AD7799_Device_t dev, uint8_t on)
{
    uint16_t mode = AD7799_MODE_IDLE | AD7799_FS_417HZ;

    if (on != 0U) {
        mode |= AD7799_MODE_PSW_ON;
    }
    AD7799_WriteMode(dev, mode);
}

uint16_t AD7799_GetNotReadyCount(AD7799_Device_t dev)
{
    return s_not_ready_cnt[(dev == AD7799_DEV_2) ? 1U : 0U];
}

void AD7799_ClearNotReadyCount(AD7799_Device_t dev)
{
    s_not_ready_cnt[(dev == AD7799_DEV_2) ? 1U : 0U] = 0U;
}



