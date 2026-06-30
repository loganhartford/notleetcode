#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_found(const char *nm, const char *in,
                       bool r, size_t got_s, size_t got_e,
                       size_t exp_s, size_t exp_e) {
    char g[32], e[32];
    snprintf(g, sizeof g, "r=%d [%zu,%zu)", r, got_s, got_e);
    snprintf(e, sizeof e, "true [%zu,%zu)", exp_s, exp_e);
    nlc_case(nm, r && got_s == exp_s && got_e == exp_e, in, e, g, "");
}
static void chk_false(const char *nm, const char *in, bool r) {
    nlc_case(nm, !r, in, "false", r ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    size_t s, e;
    bool r;

    /* found at start: {1,2,3,4,5}, target=6 → [0,3) */
    uint32_t a1[] = {1,2,3,4,5};
    r = subarray_sum_nonneg(a1, 5, 6, &s, &e);
    chk_found("sum at start", "target=6", r, s, e, 0, 3);

    /* found in middle: {1,2,3,4,5}, target=9 → [1,4) */
    uint32_t a2[] = {1,2,3,4,5};
    r = subarray_sum_nonneg(a2, 5, 9, &s, &e);
    chk_found("sum in middle", "target=9", r, s, e, 1, 4);

    /* found at end: {1,2,3,4,5}, target=12 → [2,5) */
    uint32_t a3[] = {1,2,3,4,5};
    r = subarray_sum_nonneg(a3, 5, 12, &s, &e);
    chk_found("sum at end", "target=12", r, s, e, 2, 5);

    /* not found */
    uint32_t a4[] = {1,2,3};
    r = subarray_sum_nonneg(a4, 3, 7, &s, &e);
    chk_false("not found", "target=7 in {1,2,3}", r);

    /* target=0 → empty subarray */
    uint32_t a5[] = {1,2,3};
    r = subarray_sum_nonneg(a5, 3, 0, &s, &e);
    chk_found("target=0 empty", "target=0", r, s, e, 0, 0);

    /* single element equals target */
    uint32_t a6[] = {3,7,2};
    r = subarray_sum_nonneg(a6, 3, 7, &s, &e);
    chk_found("single elem", "target=7 in {3,7,2}", r, s, e, 1, 2);

    nlc_end();
    return 0;
}
