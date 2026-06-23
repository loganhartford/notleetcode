#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int* nums, int n, const char* input, int exp) {
    nlc_capture_begin();
    int got = maxSubArray(nums, n);
    char* out = nlc_capture_end();
    char g[32], e[32];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4}; check("example 1", a, 9, "nums=[-2, 1, -3, 4, -1, 2, 1, -5, 4]", 6); }
    { int a[] = {1};          check("single", a, 1, "nums=[1]", 1); }
    { int a[] = {5, 4, -1, 7, 8}; check("all positive", a, 5, "nums=[5, 4, -1, 7, 8]", 23); }
    { int a[] = {-3, -1, -2};  check("all negative", a, 3, "nums=[-3, -1, -2]", -1); }
    { int a[] = {-5};          check("single negative", a, 1, "nums=[-5]", -5); }
    { int a[] = {-2, 0, -1};   check("zeros and neg", a, 3, "nums=[-2, 0, -1]", 0); }
    nlc_end();
    return 0;
}
