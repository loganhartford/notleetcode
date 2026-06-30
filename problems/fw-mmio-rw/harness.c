#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MMIO_BASE  0x1000u
#define MMIO_SIZE  0x100u
static uint8_t _fake_mem[MMIO_SIZE];

#include "nlc.h"
#include "solution.c"

static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
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

    memset(_fake_mem, 0, sizeof _fake_mem);
    uint32_t v;

    /* aligned write/read round-trip */
    chk_bool("write ok", "mmio_write32(0x1000, 0x12345678)",
             mmio_write32(MMIO_BASE, 0x12345678u), true);
    chk_bool("read ok",  "mmio_read32(0x1000, &v)",
             mmio_read32(MMIO_BASE, &v), true);
    chk_u32("round-trip", "read back value", v, 0x12345678u);

    /* little-endian byte order */
    chk_byte("LE byte0", "_fake_mem[0]", _fake_mem[0], 0x78);
    chk_byte("LE byte1", "_fake_mem[1]", _fake_mem[1], 0x56);
    chk_byte("LE byte2", "_fake_mem[2]", _fake_mem[2], 0x34);
    chk_byte("LE byte3", "_fake_mem[3]", _fake_mem[3], 0x12);

    /* unaligned → false */
    chk_bool("unaligned +1 false", "mmio_write32(0x1001,0)",
             mmio_write32(MMIO_BASE + 1, 0), false);
    chk_bool("unaligned +2 false", "mmio_write32(0x1002,0)",
             mmio_write32(MMIO_BASE + 2, 0), false);

    /* out of range low */
    chk_bool("below range false", "mmio_write32(0x0FFC,0)",
             mmio_write32(MMIO_BASE - 4, 0), false);

    /* out of range high (last valid = MMIO_BASE + MMIO_SIZE - 4) */
    chk_bool("last valid write", "mmio_write32(last)",
             mmio_write32(MMIO_BASE + MMIO_SIZE - 4, 0xDEADBEEFu), true);
    chk_bool("past end false", "mmio_write32(past end)",
             mmio_write32(MMIO_BASE + MMIO_SIZE, 0), false);

    /* read at different offset */
    mmio_write32(MMIO_BASE + 8, 0xAABBCCDDu);
    mmio_read32(MMIO_BASE + 8, &v);
    chk_u32("offset write/read", "write/read at +8", v, 0xAABBCCDDu);

    nlc_end();
    return 0;
}
