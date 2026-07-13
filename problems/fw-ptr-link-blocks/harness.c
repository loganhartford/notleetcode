#include <stdint.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, void *got, void *exp) {
    nlc_case(nm, got == exp, in, "correct addr", got == exp ? "correct addr" : "wrong addr", "");
}

int main(void) {
    nlc_begin();

    static uint8_t a[16] __attribute__((aligned(8)));
    static uint8_t b[16] __attribute__((aligned(8)));
    static uint8_t c[16] __attribute__((aligned(8)));

    link_blocks(a, b);
    chk("a->b",    "link_blocks(a,b)",    *(void **)a, b);

    link_blocks(b, c);
    chk("b->c",    "link_blocks(b,c)",    *(void **)b, c);

    link_blocks(c, NULL);
    chk("c->NULL", "link_blocks(c,NULL)", *(void **)c, NULL);

    /* Re-link: a->c */
    link_blocks(a, c);
    chk("a->c re-link", "link_blocks(a,c)", *(void **)a, c);

    nlc_end();
    return 0;
}
