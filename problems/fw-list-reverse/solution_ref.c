// Time: O(n) — single pass relinking nodes in place
#include <stddef.h>

node_t *list_reverse(node_t *head) {
    node_t *prev = NULL;
    node_t *cur  = head;
    while (cur) {
        node_t *nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur  = nxt;
    }
    return prev;
}
