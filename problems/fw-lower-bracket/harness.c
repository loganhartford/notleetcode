#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_ok(const char *nm, const char *in, size_t got, size_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_false(const char *nm, const char *in, bool got) {
    nlc_case(nm, !got, in, "false", got ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    const int32_t xs[] = {10, 20, 30, 40};
    size_t idx;

    /* exact matches */
    lower_bracket_index(xs, 4, 10, &idx); chk_ok("exact first", "target=10", idx, 0);
    lower_bracket_index(xs, 4, 20, &idx); chk_ok("exact mid",   "target=20", idx, 1);
    lower_bracket_index(xs, 4, 30, &idx); chk_ok("exact mid2",  "target=30", idx, 2);
    lower_bracket_index(xs, 4, 40, &idx); chk_ok("exact last",  "target=40", idx, 3);

    /* between */
    lower_bracket_index(xs, 4, 25, &idx); chk_ok("between 20-30", "target=25", idx, 1);
    lower_bracket_index(xs, 4, 39, &idx); chk_ok("between 30-40", "target=39", idx, 2);

    /* above last → n-1 */
    lower_bracket_index(xs, 4, 99, &idx); chk_ok("above range", "target=99", idx, 3);

    /* below range */
    chk_false("below range", "target=9", lower_bracket_index(xs, 4, 9,  &idx));

    /* empty */
    chk_false("empty",       "n=0",      lower_bracket_index(xs, 0, 10, &idx));

    /* duplicates in xs */
    const int32_t xdup[] = {0, 5, 5, 10};
    lower_bracket_index(xdup, 4, 5, &idx);
    /* largest i where xdup[i] <= 5; could be 1 or 2 */
    nlc_case("dup: idx>=1", idx >= 1 && idx <= 2, "dup xs target=5",
             "1 or 2", idx == 1 ? "1" : (idx == 2 ? "2" : "other"), "");

    nlc_end();
    return 0;
}
