#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int32_t a, int32_t b, int frac, int32_t exp) {
    char input[64], g[24], e[24];
    snprintf(input, sizeof input, "qmul(%d, %d, %d)", a, b, frac);
    nlc_capture_begin();
    int32_t got = qmul(a, b, frac);
    char* out = nlc_capture_end();
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("Q16: 1.5 * 2.0 = 3.0", 98304, 131072, 16, 196608);
    check("Q16: -1.5 * 2.0 = -3.0", -98304, 131072, 16, -196608);
    check("Q16: 0.5 * 0.5 = 0.25", 32768, 32768, 16, 16384);
    check("Q8: 2.5 * 4.0 = 10.0", 640, 1024, 8, 2560);
    check("Q16: 1.0 * 1.0 = 1.0", 65536, 65536, 16, 65536);
    check("Q16: 0 * x = 0", 0, 12345, 16, 0);
    nlc_end();
    return 0;
}
