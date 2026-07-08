#include <stddef.h>
#include <stdbool.h>


typedef struct node { struct node *next; int value; } node_t;

void list_push_front(node_t **head, node_t *node) {
    // TODO
}

node_t *list_pop_front(node_t **head) {
    // TODO
    return NULL;
}

bool list_remove(node_t **head, node_t *node) {
    // TODO
    return false;
}

size_t list_count(const node_t *head) {
    // TODO
    return 0;
}
