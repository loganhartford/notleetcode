#include <stdint.h>
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

    store_pointer(slot, &x);
    chk("store &x",    "store_pointer(slot, &x)",    *(void **)slot, &x);

    store_pointer(slot, &y);
    chk("store &y",    "store_pointer(slot, &y)",    *(void **)slot, &y);

    store_pointer(slot, &d);
    chk("store &d",    "store_pointer(slot, &d)",    *(void **)slot, &d);

    store_pointer(slot, NULL);
    chk("store NULL",  "store_pointer(slot, NULL)",  *(void **)slot, NULL);

    /* Offset into the buffer — verify only the target word is written */
    uint8_t *slot2 = slot + sizeof(void *);
    store_pointer(slot2, &x);
    chk("offset slot", "store_pointer(slot+ptr, &x)", *(void **)slot2, &x);

    nlc_end();
    return 0;
}
