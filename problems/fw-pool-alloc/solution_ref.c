// Time: O(1) alloc/free, O(n) init
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

bool pool_init(pool_t *pool, void *storage, size_t storage_size, size_t block_size) {
    if (block_size < sizeof(void *)) return false;
    if (storage_size < block_size)   return false;

    size_t n = storage_size / block_size;
    pool->block_size = block_size;
    pool->total      = n;
    pool->free_count = n;

    /* Build free list: each block's first word points to next block */
    uint8_t *base = (uint8_t *)storage;
    for (size_t i = 0; i < n - 1; i++) {
        void *next = base + (i + 1) * block_size;
        memcpy(base + i * block_size, &next, sizeof(void *));
    }
    void *null_ptr = NULL;
    memcpy(base + (n - 1) * block_size, &null_ptr, sizeof(void *));
    pool->free_list = base;
    return true;
}

void *pool_alloc(pool_t *pool) {
    if (!pool->free_list) return NULL;
    void *block = pool->free_list;
    void *next;
    memcpy(&next, block, sizeof(void *));
    pool->free_list = next;
    pool->free_count--;
    return block;
}

bool pool_free(pool_t *pool, void *ptr) {
    if (!ptr) return false;
    memcpy(ptr, &pool->free_list, sizeof(void *));
    pool->free_list = ptr;
    pool->free_count++;
    return true;
}

size_t pool_free_count(const pool_t *pool) {
    return pool->free_count;
}
