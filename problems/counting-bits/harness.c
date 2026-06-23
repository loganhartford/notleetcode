#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int n, const int* exp, int en) {
    char input[24];
    snprintf(input, sizeof input, "n=%d", n);
    int rs = 0;
    nlc_capture_begin();
    int* r = countBits(n, &rs);
    char* out = nlc_capture_end();
    int ok = r && rs == en;
    for (int i = 0; ok && i < en; i++) if (r[i] != exp[i]) ok = 0;
    char gb[512], eb[512];
    if (r) nlc_fmt_ints(gb, sizeof gb, r, rs);
    else snprintf(gb, sizeof gb, "NULL");
    nlc_fmt_ints(eb, sizeof eb, exp, en);
    nlc_case(name, ok, input, eb, gb, out);
    free(r);
    free(out);
}

int main(void) {
    nlc_begin();
    { int e[] = {0, 1, 1}; check("n=2", 2, e, 3); }
    { int e[] = {0, 1, 1, 2, 1, 2}; check("n=5", 5, e, 6); }
    { int e[] = {0}; check("n=0", 0, e, 1); }
    { int e[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4}; check("n=15", 15, e, 16); }
    nlc_end();
    return 0;
}
