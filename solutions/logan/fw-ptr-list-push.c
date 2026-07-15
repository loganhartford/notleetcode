#include <stdint.h>

void free_list_push(void **head, void *block) {
    // Head is the pointer to a memory location, that contains a pointer to the next allocated
    void * free = *head;
    *head = block;
    *(void **)block = free;
}
