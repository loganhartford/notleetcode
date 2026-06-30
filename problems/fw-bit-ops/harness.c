#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_u32(const char *nm, const char *in, uint32_t got, uint32_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    /* bit_set */
    chk_u32("set bit 0",        "bit_set(0x00000000, 0)",  bit_set(0x00000000u, 0),  0x00000001u);
    chk_u32("set bit 7",        "bit_set(0x00000000, 7)",  bit_set(0x00000000u, 7),  0x00000080u);
    chk_u32("set bit 31",       "bit_set(0x00000000, 31)", bit_set(0x00000000u, 31), 0x80000000u);
    chk_u32("set already-set",  "bit_set(0xFFFFFFFF, 7)",  bit_set(0xFFFFFFFFu, 7),  0xFFFFFFFFu);
    chk_u32("set n>=32 noop",   "bit_set(0x12345678, 32)", bit_set(0x12345678u, 32), 0x12345678u);

    /* bit_clear */
    chk_u32("clear bit 0",      "bit_clear(0x000000FF, 0)",  bit_clear(0x000000FFu, 0),  0x000000FEu);
    chk_u32("clear bit 7",      "bit_clear(0x000000FF, 7)",  bit_clear(0x000000FFu, 7),  0x0000007Fu);
    chk_u32("clear bit 31",     "bit_clear(0x80000000, 31)", bit_clear(0x80000000u, 31), 0x00000000u);
    chk_u32("clear already-clr","bit_clear(0x00000000, 5)",  bit_clear(0x00000000u, 5),  0x00000000u);
    chk_u32("clear n>=32 noop", "bit_clear(0xABCDEF01, 33)",bit_clear(0xABCDEF01u, 33), 0xABCDEF01u);

    /* bit_toggle */
    chk_u32("toggle bit 0 on",  "bit_toggle(0x00000000, 0)",  bit_toggle(0x00000000u, 0),  0x00000001u);
    chk_u32("toggle bit 0 off", "bit_toggle(0x00000001, 0)",  bit_toggle(0x00000001u, 0),  0x00000000u);
    chk_u32("toggle bit 31",    "bit_toggle(0x00000000, 31)", bit_toggle(0x00000000u, 31), 0x80000000u);
    chk_u32("toggle n>=32 noop","bit_toggle(0xDEADBEEF, 32)",bit_toggle(0xDEADBEEFu, 32), 0xDEADBEEFu);

    /* bit_test */
    chk_bool("test bit 0 true",  "bit_test(0x00000001, 0)", bit_test(0x00000001u, 0),  true);
    chk_bool("test bit 0 false", "bit_test(0x00000000, 0)", bit_test(0x00000000u, 0),  false);
    chk_bool("test bit 31 true", "bit_test(0x80000000, 31)",bit_test(0x80000000u, 31), true);
    chk_bool("test n>=32 false", "bit_test(0xFFFFFFFF, 32)",bit_test(0xFFFFFFFFu, 32), false);

    nlc_end();
    return 0;
}
