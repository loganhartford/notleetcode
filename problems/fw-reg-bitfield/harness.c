#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, uint32_t got, uint32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    /* reg_set_bits */
    chk("set lsb",          "reg_set_bits(0x00000000, 0x1)",      reg_set_bits(0x00000000u, 0x1u),       0x00000001u);
    chk("set bit8",         "reg_set_bits(0x00000000, 0x100)",    reg_set_bits(0x00000000u, 0x100u),     0x00000100u);
    chk("set preserves",    "reg_set_bits(0xFFFF0000, 0x0000FFFF)",reg_set_bits(0xFFFF0000u,0x0000FFFFu),0xFFFFFFFFu);

    /* reg_clear_bits */
    chk("clear lsb",        "reg_clear_bits(0xFF, 0x01)",         reg_clear_bits(0xFFu, 0x01u),          0x000000FEu);
    chk("clear upper byte", "reg_clear_bits(0xFFFFFFFF,0xFF000000)",reg_clear_bits(0xFFFFFFFFu,0xFF000000u),0x00FFFFFFu);
    chk("clear all",        "reg_clear_bits(0xFFFFFFFF,0xFFFFFFFF)",reg_clear_bits(0xFFFFFFFFu,0xFFFFFFFFu),0x00000000u);

    /* reg_write_field */
    chk("write field byte", "reg_write_field(0xFFFF0000,0x0000FF00,8,0xAB)",
        reg_write_field(0xFFFF0000u, 0x0000FF00u, 8, 0xABu), 0xFFFFAB00u);
    chk("write field zero", "reg_write_field(0xFFFFFFFF,0x000000FF,0,0x00)",
        reg_write_field(0xFFFFFFFFu, 0x000000FFu, 0, 0x00u), 0xFFFFFF00u);
    chk("write field overflow clipped","reg_write_field(0x0,0x0F,0,0xFF)",
        reg_write_field(0x0u, 0x0Fu, 0, 0xFFu), 0x0000000Fu);

    /* reg_read_field */
    chk("read field byte",  "reg_read_field(0x0000AB00,0x0000FF00,8)",
        reg_read_field(0x0000AB00u, 0x0000FF00u, 8), 0x000000ABu);
    chk("read field zero",  "reg_read_field(0x00000000,0x000000FF,0)",
        reg_read_field(0x00000000u, 0x000000FFu, 0), 0x00000000u);
    chk("read field high",  "reg_read_field(0xAB000000,0xFF000000,24)",
        reg_read_field(0xAB000000u, 0xFF000000u, 24), 0x000000ABu);

    nlc_end();
    return 0;
}
