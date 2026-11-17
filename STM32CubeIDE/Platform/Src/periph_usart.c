#include "periph_usart.h"

// Флаги занятости каналов DMA (USART1_TX = Ch4, USART2_TX = Ch7, USART3_TX = Ch2)
static volatile uint8_t tx1_busy = 0;
static volatile uint8_t tx2_busy = 0;
static volatile uint8_t tx3_busy = 0;

void USART_Platform_Init(void) {
    tx1_busy = 0;
    tx2_busy = 0;
    tx3_busy = 0;
}

void USART_Transmit_Blocking(USART_TypeDef *USARTx, uint8_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        while (!LL_USART_IsActiveFlag_TXE(USARTx));
        LL_USART_TransmitData8(USARTx, data[i]);
    }
    while (!LL_USART_IsActiveFlag_TC(USARTx));
}

uint8_t USART_Transmit_DMA(USART_TypeDef *USARTx, uint8_t *data, uint16_t len) {
    if (USARTx == USART1) {
        if (tx1_busy) return 0;
        tx1_busy = 1;
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
        LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)data);
        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, len);
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
        LL_USART_EnableDMAReq_TX(USART1);
    }
    else if (USARTx == USART2) {
        if (tx2_busy) return 0;
        tx2_busy = 1;
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
        LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_7, (uint32_t)data);
        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, len);
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
        LL_USART_EnableDMAReq_TX(USART2);
    }
    else if (USARTx == USART3) {
        if (tx3_busy) return 0;
        tx3_busy = 1;
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
        LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)data);
        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, len);
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
        LL_USART_EnableDMAReq_TX(USART3);
    }
    return 1;
}

uint8_t USART_IsTxBusy(USART_TypeDef *USARTx) {
    if (USARTx == USART1) return tx1_busy;
    if (USARTx == USART2) return tx2_busy;
    if (USARTx == USART3) return tx3_busy;
    return 0;
}

void USART_EnableIdleIT(USART_TypeDef *USARTx) {
    LL_USART_ClearFlag_IDLE(USARTx);
    LL_USART_EnableIT_IDLE(USARTx);
}
