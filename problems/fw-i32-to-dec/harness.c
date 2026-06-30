#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "nlc.h"
#include "solution.c"

static void chk_ok(const char *nm, int32_t val, size_t bufsz, const char *exp) {
    char buf[32] = {0};
    bool r = i32_to_dec(buf, bufsz, val);
    char desc[32];
    snprintf(desc, sizeof desc, "i32_to_dec(%d)", (int)val);
    nlc_case(nm, r && strcmp(buf, exp) == 0, desc, exp, r ? buf : "false", "");
}
static void chk_fail(const char *nm, int32_t val, size_t bufsz) {
    char buf[32] = {0};
    bool r = i32_to_dec(buf, bufsz, val);
    char desc[32];
    snprintf(desc, sizeof desc, "i32_to_dec(%d,bufsz=%zu)", (int)val, bufsz);
    nlc_case(nm, !r, desc, "false", r ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    chk_ok("0",              0,           16, "0");
    chk_ok("1",              1,           16, "1");
    chk_ok("-1",            -1,           16, "-1");
    chk_ok("42",            42,           16, "42");
    chk_ok("-42",          -42,           16, "-42");
    chk_ok("INT32_MAX", INT32_MAX,        16, "2147483647");
    chk_ok("INT32_MIN", INT32_MIN,        16, "-2147483648");
    chk_ok("buf exact 3",   42,            3, "42");
    chk_ok("buf exact 2",    0,            2, "0");

    chk_fail("buf too small 2",  42,  2);
    chk_fail("buf size 0",        0,  0);
    chk_fail("buf size 1 for 0",  0,  1);

    nlc_end();
    return 0;
}
