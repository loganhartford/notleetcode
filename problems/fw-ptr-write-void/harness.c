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

    int x = 0;   write_int(&x, 99);  chk("write 99",  "write_int(&x,99)",  x, 99);
    int y = 5;   write_int(&y, -3);  chk("write -3",  "write_int(&y,-3)",  y, -3);
    int z = 100; write_int(&z, 0);   chk("write 0",   "write_int(&z,0)",   z, 0);
    int a = 1;   write_int(&a, 1);   chk("same val",  "write_int(&a,1)",   a, 1);

    nlc_end();
    return 0;
}
