#ifndef AD7799_H
#define AD7799_H
#include "main.h"
#include <stdint.h>
#include <stdbool.h>

// Communication Register команды (Table 9/10, AD7799 Rev.B)
#define AD7799_CMD_RD_STATUS    0x40
#define AD7799_CMD_WR_MODE      0x08
#define AD7799_CMD_RD_MODE      0x48
#define AD7799_CMD_WR_CONFIG    0x10
#define AD7799_CMD_RD_CONFIG    0x50
#define AD7799_CMD_RD_DATA      0x58
#define AD7799_CMD_RD_ID        0x60
#define AD7799_CMD_WR_OFFSET    0x30
#define AD7799_CMD_RD_OFFSET    0x70
#define AD7799_CMD_WR_IO        0x28
#define AD7799_CMD_RD_IO        0x68
#define AD7799_CMD_WR_FS        0x38
#define AD7799_CMD_RD_FS        0x78

// ID значений
#define AD7799_ID_AD7799        0x09
#define AD7799_ID_AD7798        0x08

// Mode Register (16 бит)
#define AD7799_MODE_CONT        0x0000
#define AD7799_MODE_SINGLE      0x2000
#define AD7799_MODE_IDLE        0x4000
#define AD7799_MODE_PDOWN       0x6000
#define AD7799_MODE_CAL_ZERO    0x8000
#define AD7799_MODE_CAL_FULL    0xA000
#define AD7799_MODE_PSW_ON      0x1000
#define AD7799_FS_417HZ         0x000F
#define AD7799_MODE_IDLE_OFF    (AD7799_MODE_IDLE | AD7799_FS_417HZ)
#define AD7799_MODE_IDLE_ON     (AD7799_MODE_IDLE | AD7799_MODE_PSW_ON | AD7799_FS_417HZ)
#define AD7799_MODE_START       (AD7799_MODE_SINGLE | AD7799_MODE_PSW_ON | AD7799_FS_417HZ)
#define AD7799_MODE_CALZ_ON     (AD7799_MODE_CAL_ZERO | AD7799_MODE_PSW_ON | AD7799_FS_417HZ)

// Config Register (16 бит)
#define AD7799_CONF_BO          0x2000
#define AD7799_CONF_UNIPOLAR    0x1000
#define AD7799_CONF_BIPOLAR     0x0000
#define AD7799_CONF_GAIN(x)     (((x) & 0x07) << 8)
#define AD7799_CONF_BUF_ON      0x0010
#define AD7799_CONF_BUF         0x0010
#define AD7799_CONF_CH_AIN1     0x0000
#define AD7799_CONF_CH_AIN2     0x0001
#define AD7799_CONF_CH_AIN3     0x0002
#define AD7799_CONF_CH_INT_TEMP 0x0007

// Алиасы для каналов
#define AD7799_CH_AIN1          0x00
#define AD7799_CH_AIN2          0x01
#define AD7799_CH_AIN3          0x02
#define AD7799_CH_INT_TEMP      0x07

typedef enum {
    AD7799_CH1 = 0x00,
    AD7799_CH2 = 0x01,
    AD7799_CH3 = 0x02,
    AD7799_CH_AVDD_MON = 0x07
} AD7799_Channel_t;

typedef enum {
    AD7799_DEV_1 = 0,
    AD7799_DEV_2 = 1
} AD7799_Device_t;

// Инициализация и сброс
void AD7799_Init(void);
void AD7799_Reset(AD7799_Device_t dev);
void AD7799_SetIdleMode(AD7799_Device_t dev);

// Чтение регистров
bool AD7799_CheckID(AD7799_Device_t dev);
uint8_t  AD7799_ReadID(AD7799_Device_t dev);
uint8_t  AD7799_ReadStatus(AD7799_Device_t dev);
uint16_t AD7799_ReadMode(AD7799_Device_t dev);
uint16_t AD7799_ReadConfig(AD7799_Device_t dev);
uint8_t  AD7799_ReadIO(AD7799_Device_t dev);
uint32_t AD7799_ReadData(AD7799_Device_t dev);
uint32_t AD7799_ReadOffset(AD7799_Device_t dev);
uint32_t AD7799_ReadFullScale(AD7799_Device_t dev);

// Запись регистров
void AD7799_WriteMode(AD7799_Device_t dev, uint16_t mode);
void AD7799_WriteConfig(AD7799_Device_t dev, uint16_t config);

// Измерение одного канала
uint32_t AD7799_MeasureChannel(AD7799_Device_t dev, uint16_t config);

// Ожидание RDY LOW
uint8_t AD7799_WaitRDY(AD7799_Device_t dev, uint32_t timeout_ms);

#endif // AD7799_H
