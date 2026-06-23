#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, uint32_t n, uint32_t exp) {
    char input[32];
    snprintf(input, sizeof input, "n=%u", n);
    nlc_capture_begin();
    uint32_t got = reverseBits(n);
    char* out = nlc_capture_end();
    char g[24], e[24];
    snprintf(g, sizeof g, "%u", got);
    snprintf(e, sizeof e, "%u", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    check("example 1", 43261596u, 964176192u);
    check("example 2", 4294967293u, 3221225471u);
    check("zero", 0u, 0u);
    check("all ones", 0xFFFFFFFFu, 0xFFFFFFFFu);
    check("one -> top bit", 1u, 0x80000000u);
    nlc_end();
    return 0;
}
