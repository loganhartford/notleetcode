#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_arr(const char *nm, const char *in,
                    const int32_t *got, const int32_t *exp, size_t n) {
    int ok = memcmp(got, exp, n * sizeof(int32_t)) == 0;
    char g[64]="", e[64]="";
    for (size_t i = 0; i < n && i < 6; i++) {
        char tmp[8];
        snprintf(tmp, sizeof tmp, "%d ", (int)got[i]); strncat(g, tmp, sizeof g - strlen(g) - 1);
        snprintf(tmp, sizeof tmp, "%d ", (int)exp[i]); strncat(e, tmp, sizeof e - strlen(e) - 1);
    }
    nlc_case(nm, ok, in, e, g, "");
}

int main(void) {
    nlc_begin();

    int32_t a[6];

    /* reverse empty */
    reverse_i32(a, 0); /* no crash */
    nlc_case("reverse empty no crash", 1, "reverse_i32(a,0)", "ok", "ok", "");

    /* reverse single */
    a[0] = 42;
    reverse_i32(a, 1);
    const int32_t s1[] = {42};
    chk_arr("reverse single", "reverse_i32 n=1", a, s1, 1);

    /* reverse even */
    int32_t ev[] = {1,2,3,4};
    reverse_i32(ev, 4);
    const int32_t rev_ev[] = {4,3,2,1};
    chk_arr("reverse even", "reverse_i32([1,2,3,4])", ev, rev_ev, 4);

    /* reverse odd */
    int32_t od[] = {1,2,3,4,5};
    reverse_i32(od, 5);
    const int32_t rev_od[] = {5,4,3,2,1};
    chk_arr("reverse odd", "reverse_i32([1,2,3,4,5])", od, rev_od, 5);

    /* rotate by 0 */
    int32_t r0[] = {1,2,3,4,5};
    rotate_left_i32(r0, 5, 0);
    const int32_t same[] = {1,2,3,4,5};
    chk_arr("rotate by 0", "rotate_left(a,5,0)", r0, same, 5);

    /* rotate by 2 */
    int32_t r2[] = {1,2,3,4,5};
    rotate_left_i32(r2, 5, 2);
    const int32_t rot2[] = {3,4,5,1,2};
    chk_arr("rotate by 2", "rotate_left(a,5,2)", r2, rot2, 5);

    /* rotate by n (same as 0) */
    int32_t rn[] = {1,2,3,4,5};
    rotate_left_i32(rn, 5, 5);
    chk_arr("rotate by n", "rotate_left(a,5,5)", rn, same, 5);

    /* rotate by n+1 (wraps to 1) */
    int32_t rn1[] = {1,2,3};
    rotate_left_i32(rn1, 3, 4);
    const int32_t rot1[] = {2,3,1};
    chk_arr("rotate k>n wraps", "rotate_left([1,2,3],3,4)", rn1, rot1, 3);

    nlc_end();
    return 0;
}
