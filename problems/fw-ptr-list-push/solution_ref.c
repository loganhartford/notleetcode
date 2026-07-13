#include <stdint.h>

void free_list_push(void **head, void *block) {
    *(void **)block = *head;
    *head = block;
}
