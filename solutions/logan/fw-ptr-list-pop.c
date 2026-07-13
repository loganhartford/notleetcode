#include <stdint.h>

void *free_list_pop(void **head) {
    if (head == NULL || *head == NULL) return NULL;
    
    void * current = *head;
    *head = *(void **)current;
    return current;
}
