#include <stdint.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk_ptr(const char *nm, const char *in, void *got, void *exp) {
    char g[32], e[32];
    snprintf(g, sizeof g, "%s", got ? (got == exp ? "correct" : "wrong") : "NULL");
    snprintf(e, sizeof e, "%s", exp ? "correct addr" : "NULL");
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    static uint8_t a[16] __attribute__((aligned(8)));
    static uint8_t b[16] __attribute__((aligned(8)));
    static uint8_t c[16] __attribute__((aligned(8)));

    /* Build A->B->C->NULL */
    *(void **)a = b;
    *(void **)b = c;
    *(void **)c = NULL;
    void *head = a;

    void *r1 = free_list_pop(&head);
    chk_ptr("pop returns A", "free_list_pop returns head",  r1,   a);
    chk_ptr("head moves B",  "head advances to B",          head, b);

    void *r2 = free_list_pop(&head);
    chk_ptr("pop returns B", "free_list_pop returns B",  r2,   b);
    chk_ptr("head moves C",  "head advances to C",       head, c);

    void *r3 = free_list_pop(&head);
    chk_ptr("pop returns C", "free_list_pop returns C",    r3,   c);
    chk_ptr("head is NULL",  "head is NULL after last pop", head, NULL);

    void *r4 = free_list_pop(&head);
    chk_ptr("pop empty NULL", "free_list_pop empty list", r4, NULL);

    nlc_end();
    return 0;
}
