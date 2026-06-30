#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node { struct node *next; int value; } node_t;

#include "nlc.h"
#include "solution.c"

static void chk_ptr(const char *nm, const char *in, const node_t *got, const node_t *exp) {
    nlc_case(nm, got == exp, in, exp ? "expected_ptr" : "NULL", got ? "got_ptr" : "NULL", "");
}
static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}
static void chk_sz(const char *nm, const char *in, size_t got, size_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_val(const char *nm, const char *in, int got, int exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    node_t a={NULL,1}, b={NULL,2}, c={NULL,3};
    node_t *head = NULL;

    /* push_front order */
    list_push_front(&head, &a);
    list_push_front(&head, &b);
    list_push_front(&head, &c);
    chk_sz("count 3", "list_count after 3 push", list_count(head), 3);
    chk_val("front is c", "head->value", head->value, 3);

    /* pop_front */
    node_t *p = list_pop_front(&head);
    chk_ptr("pop returns c", "list_pop_front", p, &c);
    chk_sz("count 2 after pop", "list_count", list_count(head), 2);
    chk_val("new front is b", "head->value", head->value, 2);

    /* pop_front empty */
    head = NULL;
    chk_ptr("pop empty NULL", "list_pop_front empty", list_pop_front(&head), NULL);

    /* remove head */
    head = NULL;
    a.next = b.next = c.next = NULL;
    list_push_front(&head, &a);
    list_push_front(&head, &b);
    list_push_front(&head, &c);
    chk_bool("remove head", "list_remove head c", list_remove(&head, &c), true);
    chk_val("new head is b", "head after remove head", head->value, 2);

    /* remove middle */
    chk_bool("remove mid", "list_remove mid b", list_remove(&head, &b), true);
    chk_val("only a left", "head after remove mid", head->value, 1);

    /* remove absent */
    chk_bool("remove absent", "list_remove not in list", list_remove(&head, &b), false);

    /* count empty */
    chk_sz("count NULL=0", "list_count(NULL)", list_count(NULL), 0);

    nlc_end();
    return 0;
}
