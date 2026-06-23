#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int* nums, int n, int exp) {
    char input[128];
    nlc_fmt_ints(input, sizeof input, nums, n);
    nlc_capture_begin();
    int got = singleNumber(nums, n);
    char* out = nlc_capture_end();
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {2, 2, 1};          check("example 1", a, 3, 1); }
    { int a[] = {4, 1, 2, 1, 2};    check("example 2", a, 5, 4); }
    { int a[] = {1};                check("single", a, 1, 1); }
    { int a[] = {-1, -1, -3};       check("negatives", a, 3, -3); }
    { int a[] = {7, 3, 7, 3, 9};    check("middle", a, 5, 9); }
    nlc_end();
    return 0;
}
