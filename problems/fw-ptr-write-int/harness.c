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

    int x = 3; set_int(&x, 10);
    chk("basic", "x=3 set_int(&x,10)", x, 10);

    int y = 0; set_int(&y, -1);
    chk("negative", "y=0 set_int(&y,-1)", y, -1);

    int z = 100; set_int(&z, 0);
    chk("set zero", "z=100 set_int(&z,0)", z, 0);

    int a = 7; set_int(&a, 7);
    chk("same value", "a=7 set_int(&a,7)", a, 7);

    nlc_end();
    return 0;
}
