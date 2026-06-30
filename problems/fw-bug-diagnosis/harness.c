#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void chk_int(const char *nm, const char *in, long got, long exp) {
    char g[32], e[32];
    snprintf(g, sizeof g, "%ld", got);
    snprintf(e, sizeof e, "%ld", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_u32hex(const char *nm, const char *in, uint32_t got, uint32_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    /* Bug 1: is_pow2 via fix_bug1 */
    chk_int("bug1: 0 is not pow2",   "fix_bug1(0)",  fix_bug1(0),  0);
    chk_int("bug1: 1 is pow2",       "fix_bug1(1)",  fix_bug1(1),  1);
    chk_int("bug1: 2 is pow2",       "fix_bug1(2)",  fix_bug1(2),  1);
    chk_int("bug1: 3 is not pow2",   "fix_bug1(3)",  fix_bug1(3),  0);
    chk_int("bug1: 4 is pow2",       "fix_bug1(4)",  fix_bug1(4),  1);
    chk_int("bug1: 7 is not pow2",   "fix_bug1(7)",  fix_bug1(7),  0);
    chk_int("bug1: 16 is pow2",      "fix_bug1(16)", fix_bug1(16), 1);

    /* Bug 2: clear bit i=2, toggle bit j=4 starting from 0xFFFFFFFF */
    /* Expected: 0xFFFFFFFF & ~(1<<2) = 0xFFFFFFFB, then ^(1<<4) = 0xFFFFFFEB */
    chk_u32hex("bug2: clear bit2 toggle bit4 on 0xFFFFFFFF",
               "fix_bug2(0xFFFFFFFF,2,4)",
               fix_bug2(0xFFFFFFFF, 2, 4),
               0xFFFFFFEBu);
    /* 0x00000000: clear bit0 (no-op), toggle bit1 → 0x00000002 */
    chk_u32hex("bug2: toggle bit1 on 0x0",
               "fix_bug2(0,0,1)",
               fix_bug2(0, 0, 1),
               0x00000002u);
    /* 0x00000007: clear bit0 → 0x6, toggle bit2 → 0x2 */
    chk_u32hex("bug2: clear bit0 toggle bit2 on 0x7",
               "fix_bug2(7,0,2)",
               fix_bug2(7, 0, 2),
               0x00000002u);

    nlc_end();
    return 0;
}
