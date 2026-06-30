#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node { struct node *next; int value; } node_t;

#include "nlc.h"
#include "solution.c"

static void chk_ptr(const char *nm, const char *in, node_t *got, node_t *exp) {
    nlc_case(nm, got == exp, in, exp ? "exp_ptr" : "NULL", got ? "got_ptr" : "NULL", "");
}
static void chk_val(const char *nm, const char *in, int got, int exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    /* NULL → NULL */
    chk_ptr("reverse NULL", "list_reverse(NULL)", list_reverse(NULL), NULL);

    /* single node */
    node_t a = {NULL, 1};
    node_t *r = list_reverse(&a);
    chk_ptr("single returns same", "list_reverse single", r, &a);
    chk_ptr("single next=NULL", "a.next after reverse", a.next, NULL);

    /* two nodes: 1→2 → 2→1 */
    node_t n1={NULL,1}, n2={NULL,2};
    n1.next = &n2;
    r = list_reverse(&n1);
    chk_ptr("two: new head=n2", "list_reverse 2-node head", r, &n2);
    chk_val("two: r->val=2", "r->value", r->value, 2);
    chk_ptr("two: r->next=n1", "r->next", r->next, &n1);
    chk_ptr("two: n1->next=NULL", "n1.next", n1.next, NULL);

    /* five nodes */
    node_t nodes[5];
    for (int i = 0; i < 5; i++) { nodes[i].value = i+1; nodes[i].next = NULL; }
    for (int i = 0; i < 4; i++) nodes[i].next = &nodes[i+1];
    r = list_reverse(&nodes[0]);
    chk_ptr("five: head is nodes[4]", "list_reverse 5-node head", r, &nodes[4]);
    chk_val("five: [0]→5", "r->value", r->value, 5);
    chk_val("five: [1]→4", "r->next->value", r->next->value, 4);
    chk_val("five: [2]→3", "[2]->value", r->next->next->value, 3);
    chk_val("five: [3]→2", "[3]->value", r->next->next->next->value, 2);
    chk_val("five: [4]→1", "[4]->value", r->next->next->next->next->value, 1);
    chk_ptr("five: tail next=NULL", "tail->next", r->next->next->next->next->next, NULL);

    nlc_end();
    return 0;
}
