// Time: O(1) push/pop_front; O(n) remove/count
#include <stddef.h>
#include <stdbool.h>

void list_push_front(node_t **head, node_t *node) {
    node->next = *head;
    *head = node;
}

node_t *list_pop_front(node_t **head) {
    if (!*head) return NULL;
    node_t *n = *head;
    *head = n->next;
    n->next = NULL;
    return n;
}

bool list_remove(node_t **head, node_t *node) {
    node_t **cur = head;
    while (*cur) {
        if (*cur == node) {
            *cur = node->next;
            node->next = NULL;
            return true;
        }
        cur = &(*cur)->next;
    }
    return false;
}

size_t list_count(const node_t *head) {
    size_t n = 0;
    while (head) { n++; head = head->next; }
    return n;
}
