// Time: O(1) per alloc — single pointer arithmetic
#include <stdint.h>
#include <stddef.h>


typedef struct {
    uint8_t *base;
    size_t   size;
    size_t   offset;
} bump_alloc_t;

void bump_init(bump_alloc_t *a, void *storage, size_t size) {
    a->base   = (uint8_t *)storage;
    a->size   = size;
    a->offset = 0;
}

void *bump_alloc_aligned(bump_alloc_t *a, size_t sz, size_t alignment) {
    size_t aligned = (a->offset + alignment - 1) & ~(alignment - 1);
    if (aligned + sz > a->size) return NULL;
    void *ptr = a->base + aligned;
    a->offset = aligned + sz;
    return ptr;
}

void bump_reset(bump_alloc_t *a) {
    a->offset = 0;
}
