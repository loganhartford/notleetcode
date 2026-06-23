#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int* nums, int n, int target,
                  const char* input, int e0, int e1) {
    int rs = 0;
    nlc_capture_begin();
    int* r = twoSum(nums, n, target, &rs);
    char* out = nlc_capture_end();

    int ok = r && rs == 2 && r[0] == e0 && r[1] == e1;
    char gbuf[64], ebuf[32];
    if (r) nlc_fmt_ints(gbuf, sizeof gbuf, r, rs);
    else snprintf(gbuf, sizeof gbuf, "NULL");
    int exp[2] = {e0, e1};
    nlc_fmt_ints(ebuf, sizeof ebuf, exp, 2);

    nlc_case(name, ok, input, ebuf, gbuf, out);
    free(r);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {2, 7, 11, 15};            check("example 1", a, 4, 9, "numbers=[2,7,11,15], target=9", 1, 2); }
    { int a[] = {2, 3, 4};                 check("example 2", a, 3, 6, "numbers=[2,3,4], target=6", 1, 3); }
    { int a[] = {-1, 0};                   check("negatives", a, 2, -1, "numbers=[-1,0], target=-1", 1, 2); }
    { int a[] = {1, 2, 3, 4, 4, 9, 56, 90}; check("adjacent at end", a, 8, 8, "numbers=[1,2,3,4,4,9,56,90], target=8", 4, 5); }
    { int a[] = {1, 5, 9};                 check("first and last", a, 3, 10, "numbers=[1,5,9], target=10", 1, 3); }
    nlc_end();
    return 0;
}
