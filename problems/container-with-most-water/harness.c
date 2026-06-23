#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int* h, int n, const char* input, int exp) {
    nlc_capture_begin();
    int got = maxArea(h, n);
    char* out = nlc_capture_end();
    char g[32], e[32];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {1, 8, 6, 2, 5, 4, 8, 3, 7}; check("example 1", a, 9, "height=[1, 8, 6, 2, 5, 4, 8, 3, 7]", 49); }
    { int a[] = {1, 1};                      check("two lines", a, 2, "height=[1, 1]", 1); }
    { int a[] = {1, 2, 3, 4, 5};             check("increasing", a, 5, "height=[1, 2, 3, 4, 5]", 6); }
    { int a[] = {2, 3, 10, 5, 7, 8, 9};      check("peak middle", a, 7, "height=[2, 3, 10, 5, 7, 8, 9]", 36); }
    { int a[] = {4, 3, 2, 1, 4};             check("flat tall", a, 5, "height=[4, 3, 2, 1, 4]", 16); }
    nlc_end();
    return 0;
}
