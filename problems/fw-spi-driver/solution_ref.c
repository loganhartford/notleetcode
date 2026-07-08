// Time: O(timeout_iters) worst case — polling loops
#include <stdint.h>


#define SPI_CTRL_ENABLE    (1u << 0)
#define SPI_STATUS_TX_RDY  (1u << 0)
#define SPI_STATUS_RX_RDY  (1u << 1)
#define SPI_STATUS_ERR     (1u << 2)
typedef struct {
    uint32_t CTRL;
    uint32_t STATUS;
    uint32_t TXD;
    uint32_t RXD;
} spi_regs_t;
typedef enum { SPI_OK=0, SPI_TIMEOUT, SPI_ERROR } spi_status_t;

void spi_init(spi_regs_t *regs, uint32_t ctrl_flags) {
    regs->CTRL = ctrl_flags | SPI_CTRL_ENABLE;
}

spi_status_t spi_transfer_byte(spi_regs_t *regs, uint8_t tx,
                                uint8_t *rx, uint32_t timeout_iters) {
    /* Poll TX ready */
    uint32_t iters = timeout_iters;
    while (!(regs->STATUS & SPI_STATUS_TX_RDY)) {
        if (iters-- == 0) return SPI_TIMEOUT;
    }
    regs->TXD = tx;

    /* Poll RX ready */
    iters = timeout_iters;
    while (!(regs->STATUS & SPI_STATUS_RX_RDY)) {
        if (iters-- == 0) return SPI_TIMEOUT;
    }

    if (regs->STATUS & SPI_STATUS_ERR) return SPI_ERROR;
    *rx = (uint8_t)regs->RXD;
    return SPI_OK;
}
