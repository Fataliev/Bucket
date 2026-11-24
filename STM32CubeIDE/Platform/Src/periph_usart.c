#include "periph_usart.h"
#include "platform_tick.h"

/* USART1_TX = DMA1_Ch4, USART2_TX = DMA1_Ch7, USART3_TX = DMA1_Ch2 */
static volatile uint8_t tx1_busy = 0;
static volatile uint8_t tx2_busy = 0;
static volatile uint8_t tx3_busy = 0;

/* MX_DMA_Init() включает только тактирование и NVIC, но НЕ настраивает каналы.
   Конфигурируем каналы здесь явно — это не зависит от регенерации CubeMX. */
static void DMA_ConfigTxChannel(uint32_t channel, USART_TypeDef *USARTx)
{
    LL_DMA_InitTypeDef dma_cfg = {0};

    LL_DMA_DisableChannel(DMA1, channel);

    dma_cfg.PeriphOrM2MSrcAddress  = (uint32_t)&USARTx->DR;
    dma_cfg.MemoryOrM2MDstAddress  = 0U;                       /* задаётся при отправке */
    dma_cfg.Direction              = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    dma_cfg.Mode                   = LL_DMA_MODE_NORMAL;
    dma_cfg.NbData                 = 1U;                       /* задаётся при отправке */
    dma_cfg.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
    dma_cfg.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;
    dma_cfg.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    dma_cfg.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    dma_cfg.Priority               = LL_DMA_PRIORITY_LOW;

    LL_DMA_Init(DMA1, channel, &dma_cfg);
}

void USART_Platform_Init(void)
{
    tx1_busy = 0U;
    tx2_busy = 0U;
    tx3_busy = 0U;

    DMA_ConfigTxChannel(LL_DMA_CHANNEL_4, USART1);
    DMA_ConfigTxChannel(LL_DMA_CHANNEL_7, USART2);
    DMA_ConfigTxChannel(LL_DMA_CHANNEL_2, USART3);

    NVIC_SetPriority(DMA1_Channel4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 6, 0));
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    NVIC_SetPriority(DMA1_Channel7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 6, 0));
    NVIC_EnableIRQ(DMA1_Channel7_IRQn);
    NVIC_SetPriority(DMA1_Channel2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
    NVIC_EnableIRQ(DMA1_Channel2_IRQn);   /* в dma.c его нет — включаем сами */
}

void USART_Transmit_Blocking(USART_TypeDef *USARTx, uint8_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        while (!LL_USART_IsActiveFlag_TXE(USARTx));
        LL_USART_TransmitData8(USARTx, data[i]);
    }
    while (!LL_USART_IsActiveFlag_TC(USARTx));
}

uint8_t USART_Transmit_DMA(USART_TypeDef *USARTx, uint8_t *data, uint16_t len) {
    if (len == 0U) return 0;

    if (USARTx == USART1) {
        if (tx1_busy) { return 0U; }
        tx1_busy = 1U;
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
        LL_DMA_ClearFlag_GI4(DMA1);
        LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)data);
        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, len);
        LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
        LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_4);
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
        LL_USART_EnableDMAReq_TX(USART1);
    }
    else if (USARTx == USART2) {
        if (tx2_busy) return 0;
        tx2_busy = 1;
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
        LL_DMA_ClearFlag_TC7(DMA1);
        LL_DMA_ClearFlag_TE7(DMA1);
        LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_7, (uint32_t)data);
        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, len);
        LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_7);
        LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_7);
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);
        LL_USART_EnableDMAReq_TX(USART2);
    }
    else if (USARTx == USART3) {
        if (tx3_busy) return 0;
        tx3_busy = 1;
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
        LL_DMA_ClearFlag_TC2(DMA1);
        LL_DMA_ClearFlag_TE2(DMA1);
        LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)data);
        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, len);
        LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
        LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);
        LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
        LL_USART_EnableDMAReq_TX(USART3);
    }
    else {
        return 0;
    }
    return 1;
}

/* --- Обработчики завершения TX-DMA (вызываются из platform_isr.c) ---
   TC = последний байт загнан в USART_DR. Для RS-485 снятие DE делать
   позже по флагу TC самого USART, а не по TC DMA. */

void USART1_TX_DMA_IRQHandler(void) {          /* DMA1, канал 4 */
    if (LL_DMA_IsActiveFlag_TC4(DMA1)) {
        LL_DMA_ClearFlag_TC4(DMA1);
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
        tx1_busy = 0;
    }
    if (LL_DMA_IsActiveFlag_TE4(DMA1)) {
        LL_DMA_ClearFlag_TE4(DMA1);
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
        tx1_busy = 0;
    }
}

void USART2_TX_DMA_IRQHandler(void) {          /* DMA1, канал 7 */
    if (LL_DMA_IsActiveFlag_TC7(DMA1)) {
        LL_DMA_ClearFlag_TC7(DMA1);
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
        tx2_busy = 0;
    }
    if (LL_DMA_IsActiveFlag_TE7(DMA1)) {
        LL_DMA_ClearFlag_TE7(DMA1);
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);
        tx2_busy = 0;
    }
}

void USART3_TX_DMA_IRQHandler(void) {          /* DMA1, канал 2 */
    if (LL_DMA_IsActiveFlag_TC2(DMA1)) {
        LL_DMA_ClearFlag_TC2(DMA1);
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
        tx3_busy = 0;
    }
    if (LL_DMA_IsActiveFlag_TE2(DMA1)) {
        LL_DMA_ClearFlag_TE2(DMA1);
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
        tx3_busy = 0;
    }
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
