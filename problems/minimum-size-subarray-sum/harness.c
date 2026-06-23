#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int target, int* nums, int n, const char* input, int exp) {
    nlc_capture_begin();
    int got = minSubArrayLen(target, nums, n);
    char* out = nlc_capture_end();
    char g[32], e[32];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {2, 3, 1, 2, 4, 3};       check("example 1", 7, a, 6, "target=7, nums=[2, 3, 1, 2, 4, 3]", 2); }
    { int a[] = {1, 4, 4};                check("single suffices", 4, a, 3, "target=4, nums=[1, 4, 4]", 1); }
    { int a[] = {1, 1, 1, 1, 1, 1, 1, 1}; check("impossible", 11, a, 8, "target=11, nums=[1, 1, 1, 1, 1, 1, 1, 1]", 0); }
    { int a[] = {1, 2, 3, 4, 5};          check("whole array", 15, a, 5, "target=15, nums=[1, 2, 3, 4, 5]", 5); }
    { int a[] = {5};                      check("exact single", 5, a, 1, "target=5, nums=[5]", 1); }
    { int a[] = {10, 2, 3};               check("front window", 6, a, 3, "target=6, nums=[10, 2, 3]", 1); }
    nlc_end();
    return 0;
}
