#include <stddef.h>
#include <stdbool.h>


typedef struct {
    void  *free_list;
    size_t block_size;
    size_t free_count;
    size_t total;
} pool_t;

bool pool_init(pool_t *pool, void *storage, size_t storage_size, size_t block_size) {
    if (storage == NULL) return false;
    if (storage_size < block_size) return false;
    if (block_size < sizeof(void *)) return false;
    pool->free_list = storage;
    pool->block_size = block_size;
    pool->total = storage_size;
    pool->free_count = pool->total / pool->block_size;
    
    printf("%d", pool->total / pool->block_size);
    
    return true;
}

void *pool_alloc(pool_t *pool) {
    // TODO
    return NULL;
}

bool pool_free(pool_t *pool, void *ptr) {
    if (pool->free_count) return true;
    return false;
}

size_t pool_free_count(const pool_t *pool) {
    return pool->free_count;
}
