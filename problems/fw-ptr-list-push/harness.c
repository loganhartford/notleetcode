#include <stdint.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk_ptr(const char *nm, const char *in, void *got, void *exp) {
    nlc_case(nm, got == exp, in, "correct addr", got == exp ? "correct addr" : "wrong addr", "");
}

int main(void) {
    nlc_begin();

    static uint8_t a[16] __attribute__((aligned(8)));
    static uint8_t b[16] __attribute__((aligned(8)));
    static uint8_t c[16] __attribute__((aligned(8)));

    /* Start: head -> B -> C -> NULL */
    *(void **)b = c;
    *(void **)c = NULL;
    void *head = b;

    /* Push A: head -> A -> B -> C -> NULL */
    free_list_push(&head, a);
    chk_ptr("head is A",  "head == a after push",   head,         a);
    chk_ptr("a->b",       "*(void**)a == b",        *(void **)a,  b);
    chk_ptr("b->c",       "*(void**)b unchanged",   *(void **)b,  c);
    chk_ptr("c->NULL",    "*(void**)c unchanged",   *(void **)c,  NULL);

    /* Push C back (after popping) — head -> C -> A -> B -> C (old tail) */
    static uint8_t d[16] __attribute__((aligned(8)));
    *(void **)d = NULL;
    void *head2 = NULL;
    free_list_push(&head2, d);
    chk_ptr("push to empty: head=d", "head==d", head2, d);
    chk_ptr("push to empty: d->NULL","d->NULL", *(void **)d, NULL);

    nlc_end();
    return 0;
}
