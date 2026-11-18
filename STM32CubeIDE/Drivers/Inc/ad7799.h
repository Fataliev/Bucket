#ifndef AD7799_H
#define AD7799_H

#include "main.h"
#include <stdint.h>

// Communication Register команды
#define AD7799_CMD_READ_ID      0xC0
#define AD7799_CMD_READ_DATA    0x58
#define AD7799_CMD_WRITE_MODE   0x08
#define AD7799_CMD_WRITE_CONFIG 0x10

// ID значений
#define AD7799_ID_AD7799        0x0A
#define AD7799_ID_AD7798        0x08

typedef enum {
    AD7799_CH1 = 0,
    AD7799_CH2 = 1
} AD7799_Channel_t;

void AD7799_Init(void);
uint8_t AD7799_CheckID(AD7799_Channel_t ch);
uint32_t AD7799_ReadData(AD7799_Channel_t ch);
uint8_t AD7799_WaitRDY(AD7799_Channel_t ch, uint32_t timeout_ms);

#endif
