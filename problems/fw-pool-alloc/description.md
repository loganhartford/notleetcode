# Static Memory Pool Allocator

Implement a fixed-block memory pool allocator that provides O(1) allocation and deallocation without using `malloc`. The pool carves a caller-supplied storage region into equal-sized blocks and manages them with a free list embedded directly in the blocks.

The `pool_t` struct is provided. Implement:

```c
bool   pool_init(pool_t *pool, void *storage, size_t storage_size, size_t block_size);
void  *pool_alloc(pool_t *pool);
bool   pool_free(pool_t *pool, void *ptr);
size_t pool_free_count(const pool_t *pool);
```

- **`pool_init`** returns `false` if `block_size < sizeof(void*)` or `storage_size < block_size`
- **`pool_alloc`** returns `NULL` when the pool is exhausted
- **`pool_free`** returns `false` for `NULL` ptr
- The free list is stored inside the blocks themselves: each free block's first bytes hold a pointer to the next free block

## Examples

**Example 1** — init, alloc, free
```
uint8_t mem[64];
pool_init(&pool, mem, 64, 16);  // 4 blocks of 16 bytes
pool_free_count(&pool)  → 4
p1 = pool_alloc(&pool); // not NULL
pool_free_count(&pool)  → 3
pool_free(&pool, p1);
pool_free_count(&pool)  → 4
```

**Example 2** — exhaustion
```
pool_init(&pool, mem, 32, 16);  // 2 blocks
pool_alloc(&pool);  pool_alloc(&pool);  // both succeed
pool_alloc(&pool);  → NULL (exhausted)
```

**Example 3** — free and reuse
```
p1 = pool_alloc(&pool);
pool_free(&pool, p1);
p2 = pool_alloc(&pool);  // reuses p1's block
p2 != NULL  → true
```

## Constraints

- No `malloc` or heap allocation
- `pool_init` must return `false` if `block_size < sizeof(void*)` (can't store the next pointer)
- `pool_free(NULL)` returns `false`
- Blocks are contiguous within `storage`; each block is exactly `block_size` bytes

**Optimal complexity:** O(1) alloc/free, O(n) init
