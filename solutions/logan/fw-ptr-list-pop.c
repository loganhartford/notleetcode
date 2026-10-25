#include <stdint.h>

void *free_list_pop(void **head) {
    if (!*head) return NULL;
    
    uint8_t *a = *head;
    
    *head = *(void **)a;
    
    return a;
}
