#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *base;
    size_t   size;
} heap_t;

void heap_init(heap_t *h, void *arena, size_t size) {
}

void *heap_alloc(heap_t *h, size_t n) {
    return NULL;
}

void heap_free(heap_t *h, void *p) {
}
