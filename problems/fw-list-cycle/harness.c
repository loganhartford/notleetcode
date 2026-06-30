#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node { struct node *next; int value; } node_t;

#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    /* NULL → false */
    chk("NULL no cycle", "list_has_cycle(NULL)", list_has_cycle(NULL), false);

    /* single node, no cycle */
    node_t a = {NULL, 1};
    chk("single no cycle", "list_has_cycle single", list_has_cycle(&a), false);

    /* two nodes, no cycle */
    node_t n1={NULL,1}, n2={NULL,2};
    n1.next = &n2;
    chk("two no cycle", "list_has_cycle 2 no cycle", list_has_cycle(&n1), false);

    /* self-loop */
    node_t sl = {NULL, 42};
    sl.next = &sl;
    chk("self-loop cycle", "self-loop", list_has_cycle(&sl), true);

    /* cycle at tail → head (3 nodes) */
    node_t c1={NULL,1}, c2={NULL,2}, c3={NULL,3};
    c1.next = &c2; c2.next = &c3; c3.next = &c1;
    chk("tail→head cycle", "3-node cycle head", list_has_cycle(&c1), true);

    /* cycle in middle */
    node_t m1={NULL,1}, m2={NULL,2}, m3={NULL,3}, m4={NULL,4};
    m1.next = &m2; m2.next = &m3; m3.next = &m4; m4.next = &m3; /* 4→3 */
    chk("mid cycle", "cycle in middle", list_has_cycle(&m1), true);

    /* 5-node acyclic */
    node_t nodes[5];
    for (int i = 0; i < 5; i++) { nodes[i].value = i; nodes[i].next = NULL; }
    for (int i = 0; i < 4; i++) nodes[i].next = &nodes[i+1];
    chk("5-node no cycle", "5-node acyclic", list_has_cycle(&nodes[0]), false);

    nlc_end();
    return 0;
}
