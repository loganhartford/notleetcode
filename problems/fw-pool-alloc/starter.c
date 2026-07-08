#include <stddef.h>
#include <stdbool.h>


typedef struct {
    void  *free_list;
    size_t block_size;
    size_t free_count;
    size_t total;
} pool_t;

bool pool_init(pool_t *pool, void *storage, size_t storage_size, size_t block_size) {
    // TODO
    return false;
}

void *pool_alloc(pool_t *pool) {
    // TODO
    return NULL;
}

bool pool_free(pool_t *pool, void *ptr) {
    // TODO
    return false;
}

size_t pool_free_count(const pool_t *pool) {
    // TODO
    return 0;
}
