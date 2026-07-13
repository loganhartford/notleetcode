#include <stdint.h>

void *free_list_pop(void **head) {
    if (!*head) return NULL;
    void *block = *head;
    *head = *(void **)block;
    return block;
}
