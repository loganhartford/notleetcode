#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int a, int b, int exp) {
    char input[32];
    snprintf(input, sizeof input, "a=%d, b=%d", a, b);
    nlc_capture_begin();
    int got = getSum(a, b);
    char* out = nlc_capture_end();
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("1+2", 1, 2, 3);
    check("2+3", 2, 3, 5);
    check("-1+1", -1, 1, 0);
    check("both negative", -7, -8, -15);
    check("with zero", 0, 42, 42);
    check("mixed", 1000, -1, 999);
    nlc_end();
    return 0;
}
