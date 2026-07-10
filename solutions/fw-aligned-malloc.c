#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

void *aligned_malloc(size_t size, size_t align) {
    void * p = malloc(size);
    
    p = p + (((uint32_t)p) % size);
    
    return p;
}

void aligned_free(void *ptr) {
    free(ptr);
}
