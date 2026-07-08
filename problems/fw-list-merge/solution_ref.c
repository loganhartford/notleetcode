// Time: O(m+n) — single pass through both lists
#include <stddef.h>


typedef struct node { struct node *next; int value; } node_t;

node_t *list_merge_sorted(node_t *a, node_t *b) {
    node_t dummy = {NULL, 0};
    node_t *tail = &dummy;
    while (a && b) {
        if (a->value <= b->value) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}
