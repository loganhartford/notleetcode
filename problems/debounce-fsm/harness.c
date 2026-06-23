#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int initial, int threshold,
                  const int* raw, int n, const int* exp) {
    char inbuf[256];
    nlc_fmt_ints(inbuf, sizeof inbuf, raw, n);
    int got[64];
    debounce_t db;
    nlc_capture_begin();
    debounce_init(&db, initial, threshold);
    for (int i = 0; i < n; i++) got[i] = debounce_update(&db, raw[i]);
    char* out = nlc_capture_end();
    int ok = 1;
    for (int i = 0; i < n; i++) if (got[i] != exp[i]) ok = 0;
    char gb[256], eb[256];
    nlc_fmt_ints(gb, sizeof gb, got, n);
    nlc_fmt_ints(eb, sizeof eb, exp, n);
    nlc_case(name, ok, inbuf, eb, gb, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int raw[] = {0, 1, 1, 0, 1, 1, 1, 1, 0};
      int exp[] = {0, 0, 0, 0, 0, 0, 1, 1, 1};
      check("threshold 3, bouncy", 0, 3, raw, 9, exp); }
    { int raw[] = {1, 0, 1, 0};
      int exp[] = {1, 1, 1, 1};
      check("threshold 2, all noise", 1, 2, raw, 4, exp); }
    { int raw[] = {0, 1, 1};
      int exp[] = {0, 1, 1};
      check("threshold 1, instant", 0, 1, raw, 3, exp); }
    { int raw[] = {0, 0, 0, 0};
      int exp[] = {0, 0, 0, 0};
      check("steady, no change", 0, 2, raw, 4, exp); }
    nlc_end();
    return 0;
}
