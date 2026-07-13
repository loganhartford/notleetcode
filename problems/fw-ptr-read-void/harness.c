#include <stdint.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, int got, int exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    int a = 42;   chk("read 42",   "read_int(&42)",   read_int(&a), 42);
    int b = 0;    chk("read 0",    "read_int(&0)",    read_int(&b), 0);
    int c = -7;   chk("read -7",   "read_int(&-7)",   read_int(&c), -7);
    int d = 1000; chk("read 1000", "read_int(&1000)", read_int(&d), 1000);

    nlc_end();
    return 0;
}
