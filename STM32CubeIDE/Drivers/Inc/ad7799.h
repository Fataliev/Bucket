#ifndef AD7799_H
#define AD7799_H

#include "main.h"
#include <stdint.h>

/* ============================================================
 * Communication Register — AD7798/AD7799 Rev.B, Table 9
 * CR7=WEN(0) | CR6=R/W | CR5..CR3=RS | CR2=CREAD | CR1..0=0
 * ============================================================ */
#define AD7799_CMD_RD_STATUS    0x40   // RS=000
#define AD7799_CMD_WR_MODE      0x08   // RS=001
#define AD7799_CMD_RD_MODE      0x48
#define AD7799_CMD_WR_CONFIG    0x10   // RS=010
#define AD7799_CMD_RD_CONFIG    0x50
#define AD7799_CMD_RD_DATA      0x58   // RS=011, CREAD=1
#define AD7799_CMD_RD_ID        0x44   // RS=100  <-- ПРАВИЛЬНАЯ команда ID
#define AD7799_CMD_WR_IO        0x28   // RS=101
#define AD7799_CMD_RD_IO        0x68
#define AD7799_CMD_RD_OFFSET    0x70   // RS=110
#define AD7799_CMD_RD_FS        0x78   // RS=111

#define AD7799_ID_AD7799        0x0A
#define AD7799_ID_AD7798        0x08

/* Mode Register (16 бит): MD[2:0]=bit15..13, PSW=bit12, FS[3:0]=bit3..0 */
#define AD7799_MODE_CONT        0x0000
#define AD7799_MODE_SINGLE      0x2000
#define AD7799_MODE_IDLE        0x4000
#define AD7799_MODE_PDOWN       0x6000
#define AD7799_MODE_CAL_ZERO    0x8000
#define AD7799_MODE_CAL_FULL    0xA000
#define AD7799_MODE_PSW_ON      0x1000
#define AD7799_FS_417HZ         0x000F   // sinc3, t=480 мс (ТЗ 2.2.1)

/* Готовые значения Mode */
#define AD7799_MODE_IDLE_OFF    (AD7799_MODE_IDLE     | AD7799_FS_417HZ)            // 0x400F
#define AD7799_MODE_IDLE_ON     (AD7799_MODE_IDLE     | AD7799_MODE_PSW_ON | AD7799_FS_417HZ) // 0x500F
#define AD7799_MODE_START       (AD7799_MODE_SINGLE   | AD7799_MODE_PSW_ON | AD7799_FS_417HZ) // 0x300F
#define AD7799_MODE_CALZ_ON     (AD7799_MODE_CAL_ZERO | AD7799_MODE_PSW_ON | AD7799_FS_417HZ) // 0x900F

/* Config Register (16 бит): BO=13, U/B=12, G=10..8, REF_DET=5, BUF=4, CH=2..0 */
#define AD7799_CONF_BO          0x2000
#define AD7799_CONF_UNIPOLAR    0x1000
#define AD7799_CONF_BIPOLAR     0x0000
#define AD7799_CONF_GAIN(x)     (((x) & 0x07) << 8)
#define AD7799_CONF_BUF         0x0010
#define AD7799_CONF_CH(ch)      ((ch) & 0x07)

#define AD7799_CH_AIN1          0x00
#define AD7799_CH_AIN2          0x01
#define AD7799_CH_AIN3          0x02
#define AD7799_CH_INT_TEMP      0x07

typedef enum { AD7799_CH1 = 0, AD7799_CH2 = 1 } AD7799_Channel_t;

typedef enum {
    AD7799_CAL_OK = 0, AD7799_CAL_TIMEOUT, AD7799_CAL_NO_REF, AD7799_CAL_ERR
} AD7799_CalResult_t;

void     AD7799_Reset(AD7799_Channel_t ch);
uint8_t  AD7799_ReadStatus(AD7799_Channel_t ch);
uint8_t  AD7799_CheckID(AD7799_Channel_t ch);          // имя сохранено под main.c
uint16_t AD7799_ReadMode(AD7799_Channel_t ch);
uint16_t AD7799_ReadConfig(AD7799_Channel_t ch);
uint8_t  AD7799_ReadIO(AD7799_Channel_t ch);
uint32_t AD7799_ReadData(AD7799_Channel_t ch);
uint32_t AD7799_ReadOffset(AD7799_Channel_t ch);
uint32_t AD7799_ReadFullScale(AD7799_Channel_t ch);
void     AD7799_WriteMode(AD7799_Channel_t ch, uint16_t mode);
void     AD7799_WriteConfig(AD7799_Channel_t ch, uint16_t config);
uint8_t  AD7799_WaitRDY(AD7799_Channel_t ch, uint32_t timeout_ms);
void     AD7799_StartConversion(AD7799_Channel_t ch, uint16_t config, uint8_t settle_ms);
void     AD7799_PowerOff(AD7799_Channel_t ch);
AD7799_CalResult_t AD7799_CalibrateZeroScale(AD7799_Channel_t ch, uint16_t config);
void     AD7799_Init(void);

#endif
