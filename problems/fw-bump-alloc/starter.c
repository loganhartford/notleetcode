#include <stdint.h>
#include <stddef.h>


typedef struct {
    uint8_t *base;
    size_t   size;
    size_t   offset;
} bump_alloc_t;

void bump_init(bump_alloc_t *a, void *storage, size_t size) {
    // TODO
}

void *bump_alloc_aligned(bump_alloc_t *a, size_t sz, size_t alignment) {
    // TODO
    return NULL;
}

void bump_reset(bump_alloc_t *a) {
    // TODO
}
