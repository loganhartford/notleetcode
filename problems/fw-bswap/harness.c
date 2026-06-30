#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk16(const char *nm, const char *in, uint16_t got, uint16_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%04X", got);
    snprintf(e, sizeof e, "0x%04X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk32(const char *nm, const char *in, uint32_t got, uint32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    chk16("bswap16(0x1234)", "bswap16_u(0x1234)", bswap16_u(0x1234), 0x3412);
    chk16("bswap16(0x0000)", "bswap16_u(0x0000)", bswap16_u(0x0000), 0x0000);
    chk16("bswap16(0xFFFF)", "bswap16_u(0xFFFF)", bswap16_u(0xFFFF), 0xFFFF);
    chk16("bswap16(0x0100)", "bswap16_u(0x0100)", bswap16_u(0x0100), 0x0001);
    chk16("bswap16(0xABCD)", "bswap16_u(0xABCD)", bswap16_u(0xABCD), 0xCDAB);

    chk32("bswap32(0x01020304)", "bswap32_u(0x01020304)", bswap32_u(0x01020304u), 0x04030201u);
    chk32("bswap32(0xAABBCCDD)", "bswap32_u(0xAABBCCDD)", bswap32_u(0xAABBCCDDu), 0xDDCCBBAAu);
    chk32("bswap32(0x00000000)", "bswap32_u(0x00000000)", bswap32_u(0x00000000u), 0x00000000u);
    chk32("bswap32(0xFFFFFFFF)", "bswap32_u(0xFFFFFFFF)", bswap32_u(0xFFFFFFFFu), 0xFFFFFFFFu);
    chk32("bswap32(0x11000000)", "bswap32_u(0x11000000)", bswap32_u(0x11000000u), 0x00000011u);

    nlc_end();
    return 0;
}
