#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, uint32_t x, uint32_t exp) {
    char input[24], g[16], e[16];
    snprintf(input, sizeof input, "0x%08X", x);
    nlc_capture_begin();
    uint32_t got = swap_endianness(x);
    char* out = nlc_capture_end();
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("0x12345678", 0x12345678u, 0x78563412u);
    check("0x000000FF", 0x000000FFu, 0xFF000000u);
    check("0xAABBCCDD", 0xAABBCCDDu, 0xDDCCBBAAu);
    check("zero", 0x00000000u, 0x00000000u);
    check("palindrome bytes", 0x11223344u, 0x44332211u);
    nlc_end();
    return 0;
}
