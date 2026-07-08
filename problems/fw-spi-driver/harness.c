#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlc.h"
#include "solution.c"

static const char *status_name(spi_status_t s) {
    switch(s) { case SPI_OK: return "SPI_OK"; case SPI_TIMEOUT: return "SPI_TIMEOUT"; default: return "SPI_ERROR"; }
}
static void chk_status(const char *nm, const char *in, spi_status_t got, spi_status_t exp) {
    nlc_case(nm, got == exp, in, status_name(exp), status_name(got), "");
}
static void chk_u32(const char *nm, const char *in, uint32_t got, uint32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_byte(const char *nm, const char *in, uint8_t got, uint8_t exp) {
    char g[8], e[8];
    snprintf(g, sizeof g, "0x%02X", got);
    snprintf(e, sizeof e, "0x%02X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    spi_regs_t regs;
    uint8_t rx;

    /* init sets ENABLE */
    spi_init(&regs, 0);
    chk_u32("init CTRL has ENABLE", "spi_init ctrl=0", regs.CTRL & SPI_CTRL_ENABLE, SPI_CTRL_ENABLE);

    /* init preserves extra flags */
    spi_init(&regs, 0x10);
    chk_u32("init CTRL has flag+ENABLE","spi_init ctrl=0x10", regs.CTRL, 0x11u);

    /* normal transfer */
    regs.STATUS = SPI_STATUS_TX_RDY | SPI_STATUS_RX_RDY;
    regs.RXD    = 0xAB;
    chk_status("normal transfer SPI_OK", "spi_transfer_byte normal",
               spi_transfer_byte(&regs, 0x55, &rx, 100), SPI_OK);
    chk_byte("received 0xAB", "rx after transfer", rx, 0xAB);
    chk_u32("TXD written", "regs.TXD after transfer", regs.TXD & 0xFF, 0x55);

    /* timeout when no TX_RDY */
    regs.STATUS = 0;
    chk_status("timeout no TX_RDY", "spi_transfer_byte timeout",
               spi_transfer_byte(&regs, 0x55, &rx, 5), SPI_TIMEOUT);

    /* error when ERR bit set (TX+RX ready but ERR) */
    regs.STATUS = SPI_STATUS_TX_RDY | SPI_STATUS_RX_RDY | SPI_STATUS_ERR;
    chk_status("error when ERR set", "spi_transfer_byte error",
               spi_transfer_byte(&regs, 0x55, &rx, 100), SPI_ERROR);

    nlc_end();
    return 0;
}
