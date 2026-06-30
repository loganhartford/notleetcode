# Aligned Bump Allocator

Implement a bump (arena) allocator that serves aligned allocations from a fixed block of memory. A bump allocator simply moves a pointer forward; deallocation is not supported individually — only a full reset.

The `bump_alloc_t` struct is provided. Implement:

```c
void  bump_init(bump_alloc_t *a, void *storage, size_t size);
void *bump_alloc_aligned(bump_alloc_t *a, size_t sz, size_t alignment);
void  bump_reset(bump_alloc_t *a);
```

- **`bump_init`** — store `base`, `size`, set `offset = 0`
- **`bump_alloc_aligned`** — align the current offset up to `alignment`, then allocate `sz` bytes; return `NULL` if insufficient space
- **`bump_reset`** — set `offset = 0`, allowing the entire region to be reused

Alignment formula: `aligned = (offset + alignment - 1) & ~(alignment - 1)`.

`alignment` is always a power of two.

## Examples

**Example 1** — basic allocations
```
bump_init(&a, mem, 64);
p1 = bump_alloc_aligned(&a, 4, 4);   // aligned to 4
p2 = bump_alloc_aligned(&a, 8, 8);   // aligned to 8
// p1 != NULL, p2 != NULL
// (uintptr_t)p1 % 4 == 0
// (uintptr_t)p2 % 8 == 0
```

**Example 2** — exhaustion
```
bump_init(&a, mem, 16);
bump_alloc_aligned(&a, 12, 1);  // ok
bump_alloc_aligned(&a, 8, 1);   → NULL (only 4 bytes left)
```

**Example 3** — reset and reuse
```
bump_init(&a, mem, 32);
p1 = bump_alloc_aligned(&a, 32, 1);
bump_reset(&a);
p2 = bump_alloc_aligned(&a, 32, 1);
// p2 == p1 (same start of buffer)
```

## Constraints

- `alignment` is always a power of two (1, 2, 4, 8, 16, ...)
- Return `NULL` if `sz == 0` would go past the end or space is insufficient after alignment padding
- `bump_reset` does not erase memory contents — it only resets the offset

**Optimal complexity:** O(1) per alloc
