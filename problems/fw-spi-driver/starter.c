#include <stdint.h>

void spi_init(spi_regs_t *regs, uint32_t ctrl_flags) {
    // TODO
}

spi_status_t spi_transfer_byte(spi_regs_t *regs, uint8_t tx,
                                uint8_t *rx, uint32_t timeout_iters) {
    // TODO
    return SPI_TIMEOUT;
}
