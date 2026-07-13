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
    pool->free_count = storage_size / pool->block_size;
    pool->total = pool->free_count;
    
    uint8_t *base = storage;
    for (int i = 0; i < pool->free_count; i++) {
        uint8_t *current = base + i * block_size;
        *(void **)current = current + block_size;
    }
    
    /*
        Plan:
        - Basically store the next address in each block
        - The address of p->free_list points to the current free section,
          the value of *p->free_list points to the next free section.
        - When allocating, just need to store *p->free_list, then zero it, the set p->free_list to the stored address
        - When freeing, write the address of the current free block, as the value block being freed and see free pointer
          to current block.
    */
    
    return true;
}

void *pool_alloc(pool_t *pool) {
    if (!pool->free_count) return NULL;

    void *new_free = (void **)pool->free_list;
    *(void **)pool->free_list = 0;
    void * allocated = pool->free_list;
    pool->free_list = new_free;
    pool->free_count--;
    return allocated;
}

bool pool_free(pool_t *pool, void *ptr) {
    if (ptr == NULL) return false;
    void * old_free = pool->free_list;
    *(void **)ptr = old_free;
    pool->free_list = ptr;
    pool->free_count++;
    return true;
}

size_t pool_free_count(const pool_t *pool) {
    return pool->free_count;
}
