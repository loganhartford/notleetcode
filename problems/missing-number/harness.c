#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int* nums, int n, int exp) {
    char input[256];
    nlc_fmt_ints(input, sizeof input, nums, n);
    nlc_capture_begin();
    int got = missingNumber(nums, n);
    char* out = nlc_capture_end();
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {3, 0, 1};                   check("missing 2", a, 3, 2); }
    { int a[] = {0, 1};                      check("missing top", a, 2, 2); }
    { int a[] = {9, 6, 4, 2, 3, 5, 7, 0, 1}; check("missing 8", a, 9, 8); }
    { int a[] = {0};                         check("missing 1", a, 1, 1); }
    { int a[] = {1};                         check("missing 0", a, 1, 0); }
    nlc_end();
    return 0;
}
