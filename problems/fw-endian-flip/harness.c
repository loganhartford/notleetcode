#include <stdio.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *name, const char *input, uint32_t got, uint32_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(name, got == exp, input, e, g, "");
}

int main(void) {
    nlc_begin();

    chk("swap 0x12345678", "FLIP_ENDIANNESS(0x12345678)",
        FLIP_ENDIANNESS(0x12345678u), 0x78563412u);
    chk("swap 0xAABBCCDD", "FLIP_ENDIANNESS(0xAABBCCDD)",
        FLIP_ENDIANNESS(0xAABBCCDDu), 0xDDCCBBAAu);
    chk("LSB to MSB", "FLIP_ENDIANNESS(0x00000001)",
        FLIP_ENDIANNESS(0x00000001u), 0x01000000u);
    chk("low byte to high byte", "FLIP_ENDIANNESS(0x000000FF)",
        FLIP_ENDIANNESS(0x000000FFu), 0xFF000000u);
    chk("zero unchanged", "FLIP_ENDIANNESS(0x00000000)",
        FLIP_ENDIANNESS(0x00000000u), 0x00000000u);
    chk("all ones unchanged", "FLIP_ENDIANNESS(0xFFFFFFFF)",
        FLIP_ENDIANNESS(0xFFFFFFFFu), 0xFFFFFFFFu);

    /* Outer-parenthesization check: used inside a larger expression. A macro
       missing its surrounding ( ) would let >> bind into the middle |-chain. */
    {
        uint32_t v = 0x11223344u;
        uint32_t got = FLIP_ENDIANNESS(v) >> 8;      /* expect 0x00443322 */
        chk("safe in larger expression (outer parens)",
            "FLIP_ENDIANNESS(0x11223344) >> 8", got, 0x00443322u);
    }

    /* Argument-parenthesization check: called with an expression argument.
       Without ((x)) around each use, & would bind before |. */
    {
        uint32_t hi = 0x12340000u, lo = 0x00005678u;
        uint32_t got = FLIP_ENDIANNESS(hi | lo);     /* expect 0x78563412 */
        chk("expression argument (inner parens)",
            "FLIP_ENDIANNESS(hi | lo)", got, 0x78563412u);
    }

    /* Applying the flip twice is the identity. */
    {
        uint32_t v = 0xDEADBEEFu;
        chk("double flip is identity", "FLIP_ENDIANNESS(FLIP_ENDIANNESS(0xDEADBEEF))",
            FLIP_ENDIANNESS(FLIP_ENDIANNESS(v)), 0xDEADBEEFu);
    }

    nlc_end();
    return 0;
}
