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

    int ok = r && rs == 2 &&
             ((r[0] == e0 && r[1] == e1) || (r[0] == e1 && r[1] == e0));
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
    { int a[] = {2, 7, 11, 15};     check("example 1", a, 4, 9, "nums=[2,7,11,15], target=9", 0, 1); }
    { int a[] = {3, 2, 4};          check("example 2", a, 3, 6, "nums=[3,2,4], target=6", 1, 2); }
    { int a[] = {3, 3};             check("duplicate values", a, 2, 6, "nums=[3,3], target=6", 0, 1); }
    { int a[] = {-1, -2, -3, -4, -5}; check("negatives", a, 5, -8, "nums=[-1,-2,-3,-4,-5], target=-8", 2, 4); }
    { int a[] = {0, 4, 3, 0};       check("spread out", a, 4, 0, "nums=[0,4,3,0], target=0", 0, 3); }
    nlc_end();
    return 0;
}
