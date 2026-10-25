#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "nlc.h"
#include "solution.c"

static void chk_sum(const char *name, const char *input,
                    unsigned a, int b, int thr, int exp) {
    int got = sum_gt(a, b, thr);
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, "");
}

static void chk_nib(const char *name, const char *input, uint8_t port, uint8_t exp) {
    uint8_t got = inv_hi_nibble(port);
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%02X", (unsigned)got);
    snprintf(e, sizeof e, "0x%02X", (unsigned)exp);
    nlc_case(name, got == exp, input, e, g, "");
}

static int count_bits(unsigned v) {
    int n = 0;
    for (; v; v >>= 1) n += (int)(v & 1u);
    return n;
}

int main(void) {
    nlc_begin();

    chk_sum("6 + (-20) is not > 6", "sum_gt(6, -20, 6)", 6u, -20, 6, 0);
    chk_sum("6 + 20 > 6", "sum_gt(6, 20, 6)", 6u, 20, 6, 1);
    chk_sum("sum equal to thr is not greater", "sum_gt(10, -4, 6)", 10u, -4, 6, 0);
    chk_sum("sum one above thr", "sum_gt(10, -3, 6)", 10u, -3, 6, 1);
    chk_sum("negative sum vs zero", "sum_gt(3, -5, 0)", 3u, -5, 0, 0);
    chk_sum("negative threshold", "sum_gt(5, 0, -1)", 5u, 0, -1, 1);
    chk_sum("sum exceeds UINT_MAX", "sum_gt(UINT_MAX, 1, 0)", UINT_MAX, 1, 0, 1);
    chk_sum("a above INT_MAX", "sum_gt(2147483648u, 0, 0)", (unsigned)INT_MAX + 1u, 0, 0, 1);
    chk_sum("extremes: sum equals INT_MAX", "sum_gt(UINT_MAX, INT_MIN, INT_MAX)",
            UINT_MAX, INT_MIN, INT_MAX, 0);

    {
        unsigned got = all_ones();
        char g[32], e[32];
        snprintf(g, sizeof g, "0x%X", got);
        snprintf(e, sizeof e, "0x%X", UINT_MAX);
        nlc_case("all_ones() == UINT_MAX", got == UINT_MAX, "all_ones()", e, g, "");
    }
    {
        int width = (int)(CHAR_BIT * sizeof(unsigned));
        int bits = count_bits(all_ones());
        char g[32], e[32];
        snprintf(g, sizeof g, "%d of %d bits set", bits, width);
        snprintf(e, sizeof e, "%d of %d bits set", width, width);
        nlc_case("every bit of unsigned int is set", bits == width, "all_ones()", e, g, "");
    }

    chk_nib("inverse of 0x5A -> 0x0A", "inv_hi_nibble(0x5A)", 0x5A, 0x0A);
    chk_nib("inverse of 0x00 -> 0x0F", "inv_hi_nibble(0x00)", 0x00, 0x0F);
    chk_nib("inverse of 0xFF -> 0x00", "inv_hi_nibble(0xFF)", 0xFF, 0x00);
    chk_nib("inverse of 0xF0 -> 0x00", "inv_hi_nibble(0xF0)", 0xF0, 0x00);
    chk_nib("inverse of 0x3C -> 0x0C", "inv_hi_nibble(0x3C)", 0x3C, 0x0C);

    nlc_end();
    return 0;
}
