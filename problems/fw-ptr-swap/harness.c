#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_int(const char *nm, const char *in, int got, int exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_dbl(const char *nm, const char *in, double got, double exp) {
    char g[32], e[32];
    snprintf(g, sizeof g, "%.1f", got);
    snprintf(e, sizeof e, "%.1f", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_ptr(const char *nm, const char *in, void *got, void *exp) {
    nlc_case(nm, got == exp, in, "correct ptr", got == exp ? "correct ptr" : "wrong ptr", "");
}

int main(void) {
    nlc_begin();

    /* ints */
    int x = 10, y = 20;
    swap_values(&x, &y, sizeof(int));
    chk_int("int x=20", "swap int 10,20 → x", x, 20);
    chk_int("int y=10", "swap int 10,20 → y", y, 10);

    /* doubles */
    double p = 1.5, q = 3.5;
    swap_values(&p, &q, sizeof(double));
    chk_dbl("double p=3.5", "swap double 1.5,3.5 → p", p, 3.5);
    chk_dbl("double q=1.5", "swap double 1.5,3.5 → q", q, 1.5);

    /* pointers */
    int a, b;
    void *pa = &a, *pb = &b;
    swap_values(&pa, &pb, sizeof(void *));
    chk_ptr("ptr pa=&b", "swap void* → pa", pa, &b);
    chk_ptr("ptr pb=&a", "swap void* → pb", pb, &a);

    /* same address: no-op */
    int z = 42;
    swap_values(&z, &z, sizeof(int));
    chk_int("self-swap", "swap(&z,&z,4)", z, 42);

    nlc_end();
    return 0;
}
