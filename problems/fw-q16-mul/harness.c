#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_q16(const char *nm, const char *in, q16_t got, q16_t exp) {
    char g[32], e[32];
    snprintf(g, sizeof g, "%d (%.4f)", (int)got, (double)got / 65536.0);
    snprintf(e, sizeof e, "%d (%.4f)", (int)exp, (double)exp / 65536.0);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_i32(const char *nm, const char *in, int32_t got, int32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", (int)got);
    snprintf(e, sizeof e, "%d", (int)exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    /* 1.0 representation */
    q16_t one  = q16_from_int(1);   /* 65536 */
    q16_t two  = q16_from_int(2);   /* 131072 */
    q16_t zero = q16_from_int(0);
    q16_t neg1 = q16_from_int(-1);  /* -65536 */

    chk_q16("from_int(1)=65536",  "q16_from_int(1)",  one,  65536);
    chk_q16("from_int(2)=131072", "q16_from_int(2)",  two,  131072);
    chk_q16("from_int(0)=0",      "q16_from_int(0)",  zero, 0);
    chk_q16("from_int(-1)=-65536","q16_from_int(-1)", neg1, -65536);

    /* to_int */
    chk_i32("to_int(one)=1",     "q16_to_int(65536)", q16_to_int(one), 1);
    chk_i32("to_int(0.5 trunc)", "q16_to_int(32768)", q16_to_int(32768), 0);
    chk_i32("to_int(3)",         "q16_to_int(3<<16)", q16_to_int(q16_from_int(3)), 3);

    /* multiplication */
    /* 1.0 * 1.0 = 1.0 */
    chk_q16("1.0*1.0=1.0", "q16_mul(one,one)", q16_mul(one, one), 65536);
    /* 1.5 * 2.0 = 3.0 */
    q16_t q1_5 = 98304;  /* 1.5 * 65536 */
    chk_q16("1.5*2.0=3.0", "q16_mul(1.5,2.0)", q16_mul(q1_5, two), 196608);
    /* 0.5 * 0.5 = 0.25 */
    q16_t half = 32768;
    chk_q16("0.5*0.5=0.25", "q16_mul(0.5,0.5)", q16_mul(half, half), 16384);
    /* -1.0 * 2.0 = -2.0 */
    chk_q16("-1.0*2.0=-2.0", "q16_mul(-1.0,2.0)", q16_mul(neg1, two), -131072);
    /* 0.0 * anything = 0.0 */
    chk_q16("0.0*1.0=0.0", "q16_mul(0,one)", q16_mul(zero, one), 0);

    nlc_end();
    return 0;
}
