#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

void *aligned_malloc(size_t size, size_t align) {
    if (align == 0) return NULL;
    /* Extra space: align bytes for padding + pointer to store original malloc ptr */
    void *raw = malloc(size + align + sizeof(void *));
    if (!raw) return NULL;
    /* Find the first aligned address that leaves room for the stored pointer */
    uintptr_t base = (uintptr_t)raw + sizeof(void *);
    uintptr_t aligned = (base + align - 1) & ~(uintptr_t)(align - 1);
    /* Store the original malloc pointer just before the aligned address */
    ((void **)aligned)[-1] = raw;
    return (void *)aligned;
}

void aligned_free(void *ptr) {
    if (!ptr) return;
    free(((void **)ptr)[-1]);
}
