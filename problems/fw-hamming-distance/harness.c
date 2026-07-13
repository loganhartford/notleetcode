#include <stdint.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, int got, int exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    chk("same values",    "hamming(5,5)",           hamming_distance(5, 5),           0);
    chk("example 5,6",   "hamming(5,6)",            hamming_distance(5, 6),           2);
    chk("1 vs 2",         "hamming(1,2)",            hamming_distance(1, 2),           2);
    chk("0 vs 0xFF",      "hamming(0,0xFF)",         hamming_distance(0, 0xFF),        8);
    chk("0 vs -1",        "hamming(0,-1)",           hamming_distance(0, -1),          32);
    chk("alternating",    "hamming(0x55555555,0xAAAAAAAA)", hamming_distance(0x55555555, (int)0xAAAAAAAA), 32);
    chk("one bit",        "hamming(0,1)",            hamming_distance(0, 1),           1);
    chk("adjacent bits",  "hamming(0b0110,0b1001)",  hamming_distance(0x6, 0x9),       4);

    nlc_end();
    return 0;
}
