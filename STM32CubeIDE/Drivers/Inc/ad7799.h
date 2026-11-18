#ifndef AD7799_H
#define AD7799_H

#include "main.h"
#include <stdint.h>

extern volatile uint8_t g_adc1_type;
extern volatile uint8_t g_adc2_type;

// Communication Register команды
#define AD7799_CMD_READ_STATUS  0x40
#define AD7799_CMD_WRITE_MODE   0x08
#define AD7799_CMD_READ_MODE    0x48
#define AD7799_CMD_WRITE_CONFIG 0x10
#define AD7799_CMD_READ_CONFIG  0x50
#define AD7799_CMD_READ_DATA    0x58
#define AD7799_CMD_READ_ID      0x60
#define AD7799_CMD_READ_IO      0x68
#define AD7799_CMD_READ_OFFSET  0x70
#define AD7799_CMD_READ_FS      0x78

// ID значений
#define AD7799_ID_AD7799        0x0A
#define AD7799_ID_AD7798        0x08

// Mode Register
#define AD7799_MODE_CONTINUOUS  0x0000
#define AD7799_MODE_SINGLE      0x2000
#define AD7799_MODE_IDLE        0x4000
#define AD7799_MODE_CAL_ZERO    0x8000
#define AD7799_MODE_PSW_ON      0x1000
#define AD7799_FS_417HZ         0x000F

#define AD7799_MODE_IDLE_OFF    (AD7799_MODE_IDLE | AD7799_FS_417HZ)
#define AD7799_MODE_IDLE_ON     (AD7799_MODE_IDLE | AD7799_MODE_PSW_ON | AD7799_FS_417HZ)
#define AD7799_MODE_START       (AD7799_MODE_SINGLE | AD7799_MODE_PSW_ON | AD7799_FS_417HZ)

// Config Register
#define AD7799_CONF_UNIPOLAR    0x1000
#define AD7799_CONF_BIPOLAR     0x0000
#define AD7799_CONF_GAIN(g)     (((g) & 0x07) << 8)
#define AD7799_CONF_BUF_ON      0x0010
#define AD7799_CONF_CH_AIN1     0x0000
#define AD7799_CONF_CH_AIN2     0x0001
#define AD7799_CONF_CH_AIN3     0x0002
#define AD7799_CONF_CH_INT_TEMP 0x0007

typedef enum {
    AD7799_CH1 = 0,
    AD7799_CH2 = 1
} AD7799_Channel_t;

void AD7799_Init(void);
uint8_t AD7799_CheckID(AD7799_Channel_t ch);

// Запись
void AD7799_WriteMode(AD7799_Channel_t ch, uint16_t mode_value);
void AD7799_WriteConfig(AD7799_Channel_t ch, uint16_t config_value);

// Чтение регистров
uint8_t  AD7799_ReadStatus(AD7799_Channel_t ch);
uint16_t AD7799_ReadMode(AD7799_Channel_t ch);
uint16_t AD7799_ReadConfig(AD7799_Channel_t ch);
uint8_t  AD7799_ReadIO(AD7799_Channel_t ch);
uint32_t AD7799_ReadData(AD7799_Channel_t ch);

// Чтение канала с запуском одиночной конверсии
// channel_config - значение Config Register (канал + усиление + BUF)
uint32_t AD7799_ReadChannel(AD7799_Channel_t ch, uint16_t channel_config);

uint8_t AD7799_WaitRDY(AD7799_Channel_t ch, uint32_t timeout_ms);

#endif // AD7799_H
