#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "nlc.h"
#include "solution.c"

static void chk_ok(const char *nm, const char *s, int32_t exp) {
    int32_t v = 0;
    bool r = parse_i32(s, &v);
    char g[32], e[32];
    snprintf(e, sizeof e, "%d", (int)exp);
    if (!r) snprintf(g, sizeof g, "false");
    else    snprintf(g, sizeof g, "%d", (int)v);
    nlc_case(nm, r && v == exp, s, e, g, "");
}
static void chk_fail(const char *nm, const char *s) {
    int32_t v = 0;
    bool r = parse_i32(s, &v);
    nlc_case(nm, !r, s, "false", r ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    chk_ok("parse 0",             "0",           0);
    chk_ok("parse 123",           "123",         123);
    chk_ok("parse -456",          "-456",        -456);
    chk_ok("parse +789",          "+789",        789);
    chk_ok("parse INT32_MAX",     "2147483647",  INT32_MAX);
    chk_ok("parse INT32_MIN",     "-2147483648", INT32_MIN);

    chk_fail("overflow",          "2147483648");
    chk_fail("underflow",         "-2147483649");
    chk_fail("empty",             "");
    chk_fail("non-digit",         "12x");
    chk_fail("leading space",     " 5");
    chk_fail("sign only +",       "+");
    chk_fail("sign only -",       "-");
    chk_fail("trailing space",    "5 ");

    nlc_end();
    return 0;
}
