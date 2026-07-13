#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, void *got, void *exp) {
    nlc_case(nm, got == exp, in, "correct addr", got == exp ? "correct addr" : "wrong addr", "");
}

int main(void) {
    nlc_begin();

    static uint8_t slot[sizeof(void *) * 4] __attribute__((aligned(8)));

    int x, y;
    double d;

    *(void **)slot = &x;
    chk("load &x",   "load after store &x",   load_pointer(slot), &x);

    *(void **)slot = &y;
    chk("load &y",   "load after store &y",   load_pointer(slot), &y);

    *(void **)slot = &d;
    chk("load &d",   "load after store &d",   load_pointer(slot), &d);

    *(void **)slot = NULL;
    chk("load NULL", "load after store NULL",  load_pointer(slot), NULL);

    nlc_end();
    return 0;
}
