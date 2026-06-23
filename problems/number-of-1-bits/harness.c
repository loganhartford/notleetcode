#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, uint32_t n, int exp) {
    char input[32];
    snprintf(input, sizeof input, "n=%u", n);
    nlc_capture_begin();
    int got = hammingWeight(n);
    char* out = nlc_capture_end();
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("eleven", 11u, 3);
    check("power of two", 128u, 1);
    check("all but one", 4294967293u, 31);
    check("zero", 0u, 0);
    check("all ones", 0xFFFFFFFFu, 32);
    check("one", 1u, 1);
    nlc_end();
    return 0;
}
