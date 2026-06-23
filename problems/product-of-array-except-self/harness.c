#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int* nums, int n, const char* input, int* exp, int en) {
    int rs = 0;
    nlc_capture_begin();
    int* r = productExceptSelf(nums, n, &rs);
    char* out = nlc_capture_end();

    int ok = r && rs == en && memcmp(r, exp, en * sizeof(int)) == 0;
    char gbuf[128], ebuf[128];
    if (r) nlc_fmt_ints(gbuf, sizeof gbuf, r, rs);
    else snprintf(gbuf, sizeof gbuf, "NULL");
    nlc_fmt_ints(ebuf, sizeof ebuf, exp, en);

    nlc_case(name, ok, input, ebuf, gbuf, out);
    free(r);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {1, 2, 3, 4};       int e[] = {24, 12, 8, 6}; check("example 1", a, 4, "nums=[1, 2, 3, 4]", e, 4); }
    { int a[] = {-1, 1, 0, -3, 3};  int e[] = {0, 0, 9, 0, 0}; check("with zero", a, 5, "nums=[-1, 1, 0, -3, 3]", e, 5); }
    { int a[] = {2, 3};             int e[] = {3, 2}; check("two elements", a, 2, "nums=[2, 3]", e, 2); }
    { int a[] = {0, 0};             int e[] = {0, 0}; check("two zeros", a, 2, "nums=[0, 0]", e, 2); }
    { int a[] = {-1, -2, -3};       int e[] = {6, 3, 2}; check("negatives", a, 3, "nums=[-1, -2, -3]", e, 3); }
    nlc_end();
    return 0;
}
