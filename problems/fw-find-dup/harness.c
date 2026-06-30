#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_dup(const char *nm, const char *in, uint32_t got, uint32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%u", got);
    snprintf(e, sizeof e, "%u", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_true(const char *nm, const char *in, bool got) {
    nlc_case(nm, got, in, "true", got ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    uint32_t dup;

    /* dup at start */
    uint32_t a1[] = {0,1,2,0};
    chk_true("dup at start ret", "find_dup {0,1,2,0}", find_duplicate_u32(a1, 4, &dup));
    chk_dup("dup at start val", "{0,1,2,0}", dup, 0);

    /* dup at end */
    uint32_t a2[] = {1,2,3,3};
    chk_true("dup at end ret",  "find_dup {1,2,3,3}", find_duplicate_u32(a2, 4, &dup));
    chk_dup("dup at end val",   "{1,2,3,3}", dup, 3);

    /* dup in middle */
    uint32_t a3[] = {1,3,2,3};
    chk_true("dup mid ret",  "find_dup {1,3,2,3}", find_duplicate_u32(a3, 4, &dup));
    chk_dup("dup mid val",   "{1,3,2,3}", dup, 3);

    /* n=2 */
    uint32_t a4[] = {0,0};
    chk_true("n=2 ret",  "find_dup {0,0}", find_duplicate_u32(a4, 2, &dup));
    chk_dup("n=2 val",   "{0,0}", dup, 0);

    /* multiple dups — find any */
    uint32_t a5[] = {2,2,1,1};
    bool r5 = find_duplicate_u32(a5, 4, &dup);
    chk_true("multi dup ret", "find_dup {2,2,1,1}", r5);
    /* dup is either 1 or 2 */
    nlc_case("multi dup val is 1 or 2", dup == 1 || dup == 2,
             "find_dup multi", "1 or 2", dup == 1 ? "1" : (dup == 2 ? "2" : "other"), "");

    nlc_end();
    return 0;
}
