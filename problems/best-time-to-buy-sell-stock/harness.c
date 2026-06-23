#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

static void check(const char* name, int* prices, int n, const char* input, int exp) {
    nlc_capture_begin();
    int got = maxProfit(prices, n);
    char* out = nlc_capture_end();
    char g[32], e[32];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {7, 1, 5, 3, 6, 4}; check("example 1", a, 6, "prices=[7, 1, 5, 3, 6, 4]", 5); }
    { int a[] = {7, 6, 4, 3, 1};    check("no profit", a, 5, "prices=[7, 6, 4, 3, 1]", 0); }
    { int a[] = {5};                check("single day", a, 1, "prices=[5]", 0); }
    { int a[] = {1, 2, 3, 4, 5};    check("increasing", a, 5, "prices=[1, 2, 3, 4, 5]", 4); }
    { int a[] = {3, 2, 6, 5, 0, 3}; check("dip then rise", a, 6, "prices=[3, 2, 6, 5, 0, 3]", 4); }
    { int a[] = {2, 4};             check("two days", a, 2, "prices=[2, 4]", 2); }
    nlc_end();
    return 0;
}
