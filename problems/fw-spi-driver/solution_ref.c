// Time: O(timeout_iters) worst case — polling loops
#include <stdint.h>

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
