#include "periph_spi.h"
#include "platform_tick.h"
#include <stddef.h>

static volatile uint8_t spi_busy = 0;
static uint8_t *spi_tx_buf = NULL;
static uint8_t *spi_rx_buf = NULL;
static uint16_t spi_len = 0;
static uint16_t spi_tx_idx = 0;
static uint16_t spi_rx_idx = 0;
static SPI_Callback_t spi_callback = NULL;
static void *spi_context = NULL;

void SPI_Platform_Init(void) {
    spi_busy = 0;
    spi_callback = NULL;
    spi_context = NULL;
}

// Блокирующая передача: одновременная передача и прием (full-duplex)
void SPI_TransmitReceive_Blocking(uint8_t *txData, uint8_t *rxData, uint16_t len) {
    /* Сброс "хвоста" предыдущей транзакции: без этого первый байт кадра
       читается из DR устаревшим (OVR) и весь обмен сдвигается на байт. */
    if (LL_SPI_IsActiveFlag_RXNE(SPI1)) {
        (void)LL_SPI_ReceiveData8(SPI1);
    }
    LL_SPI_ClearFlag_OVR(SPI1);

    for (uint16_t i = 0; i < len; i++) {
        uint32_t t0 = GetTick();
        while (!LL_SPI_IsActiveFlag_TXE(SPI1)) {
            if ((uint32_t)(GetTick() - t0) >= 10) return;  /* защита от зависания */
        }
        LL_SPI_TransmitData8(SPI1, txData ? txData[i] : 0xFF);

        t0 = GetTick();
        while (!LL_SPI_IsActiveFlag_RXNE(SPI1)) {
            if ((uint32_t)(GetTick() - t0) >= 10) return;
        }
        /* DR читаем ВСЕГДА, даже если буфер приёма не нужен */
        if (rxData) rxData[i] = LL_SPI_ReceiveData8(SPI1);
        else        (void)LL_SPI_ReceiveData8(SPI1);
    }
    while (LL_SPI_IsActiveFlag_BSY(SPI1));
}


uint8_t SPI_TransmitReceive_IT(uint8_t *txData, uint8_t *rxData, uint16_t len,
                               SPI_Callback_t callback, void *context) {
    if (spi_busy) return 0;

    spi_busy = 1;
    spi_tx_buf = txData;
    spi_rx_buf = rxData;
    spi_len = len;
    spi_tx_idx = 0;
    spi_rx_idx = 0;
    spi_callback = callback;
    spi_context = context;

    LL_SPI_ClearFlag_OVR(SPI1);
    (void)LL_SPI_ReceiveData8(SPI1);

    LL_SPI_EnableIT_TXE(SPI1);
    LL_SPI_EnableIT_RXNE(SPI1);
    LL_SPI_EnableIT_ERR(SPI1);

    return 1;
}

uint8_t SPI_IsBusy(void) {
    return spi_busy;
}

void SPI_IRQ_Handler(void) {
    if (LL_SPI_IsActiveFlag_OVR(SPI1)) {
        LL_SPI_ClearFlag_OVR(SPI1);
        (void)LL_SPI_ReceiveData8(SPI1);
    }

    if (LL_SPI_IsActiveFlag_RXNE(SPI1)) {
        uint8_t data = LL_SPI_ReceiveData8(SPI1);
        if (spi_rx_buf && spi_rx_idx < spi_len) {
            spi_rx_buf[spi_rx_idx] = data;
        }
        spi_rx_idx++;
    }

    if (LL_SPI_IsActiveFlag_TXE(SPI1)) {
        if (spi_tx_idx < spi_len) {
            uint8_t tx_byte = spi_tx_buf ? spi_tx_buf[spi_tx_idx] : 0xFF;
            LL_SPI_TransmitData8(SPI1, tx_byte);
            spi_tx_idx++;
        }
    }

    if (spi_tx_idx >= spi_len && spi_rx_idx >= spi_len) {
        LL_SPI_DisableIT_TXE(SPI1);
        LL_SPI_DisableIT_RXNE(SPI1);
        LL_SPI_DisableIT_ERR(SPI1);
        spi_busy = 0;

        if (spi_callback) {
            spi_callback(spi_context, spi_rx_buf, spi_len);
        }
    }
}
