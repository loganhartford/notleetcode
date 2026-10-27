# First-Fit Heap Allocator

Implement `malloc` and `free` for a system with no heap of its own. The caller provides one block of memory, the arena, and every allocation comes out of it. All of your bookkeeping must live inside the arena or in the two fields of `heap_t`: no `malloc`, and no static or global variables, since several heaps can be in use at once.

```c
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t *base;
    size_t   size;
} heap_t;

void  heap_init(heap_t *h, void *arena, size_t size);
void *heap_alloc(heap_t *h, size_t n);
void  heap_free(heap_t *h, void *p);
```

- **`heap_init`** makes `h` an empty heap over the `size` bytes at `arena`. `*h` and the arena may hold garbage beforehand, or a heap that was already in use, whose blocks are then discarded.
- **`heap_alloc`** returns a pointer to `n` bytes the caller may use until it frees them: 8-byte aligned, inside the arena, and not overlapping any other live block. It returns `NULL` when `n == 0` or when no free space can hold the request; a request larger than the arena always gets `NULL`.
- **`heap_free`** gives a block returned by `heap_alloc` back to the heap. `heap_free(h, NULL)` does nothing. Freeing any other pointer, or freeing a block twice, is outside the contract.
- The heap never writes to the bytes of a live block, or to any byte outside the arena.

**Placement is first fit by address.** Each new block goes at the lowest address where free space can hold it. The space a freed block leaves can always hold a request up to that block's `n`, so when nothing lower is free, the new block gets the freed block's old address.

**Space guarantees.** Let `r(n)` be `n` rounded up to a multiple of 8.

1. At most 16 bytes of bookkeeping per block: starting from an empty heap (just initialized, or with every block freed), requests `n1, n2, ..., nk` all succeed whenever `(r(n1) + 16) + ... + (r(nk) + 16) <= size`.
2. Freed space is never lost: once every block has been freed, `heap_alloc(h, size - 16)` succeeds.
3. Freed neighbours act as one space: if blocks `p1, ..., pk` sit side by side in memory with no other block between them (as consecutive allocations from an empty heap do), then once all of them are freed, in any order, a request for `r(n1) + ... + r(nk)` bytes fits at `p1`, and so returns `p1` when nothing lower fits it.

Python twin: the arena is a `bytearray` and blocks are byte offsets into it instead of pointers. `heap_init(h, arena)` keeps the arena in `h.arena` (`h` is the `Heap` from the starter), `heap_alloc(h, n)` returns an offset that is a multiple of 8, or `None`, and `heap_free(h, None)` does nothing.

### Example 1
```
heap_init(&h, arena, 1024);
a = heap_alloc(&h, 64);      -> non-NULL, a % 8 == 0
b = heap_alloc(&h, 128);     -> b > a
c = heap_alloc(&h, 64);
heap_free(&h, b);
heap_alloc(&h, 64)           -> b       (the lowest free space that fits)
heap_alloc(&h, 0)            -> NULL
heap_alloc(&h, 2000)         -> NULL    (larger than the arena)
```

### Example 2
```
heap_init(&h, arena, 512);
p = heap_alloc(&h, 37);      -> r(37) = 40
q = heap_alloc(&h, 20);      -> r(20) = 24; q sits right after p
g = heap_alloc(&h, 8);
heap_free(&h, q);
heap_free(&h, p);
heap_alloc(&h, 64)           -> p       (40 + 24: the freed neighbours act as one space)
```

### Example 3
```
heap_init(&h, arena, 4096);
heap_alloc(&h, 16) until it returns NULL   -> at least 4096 / (16 + 16) = 128 succeed
free every block, in any order
heap_alloc(&h, 4096 - 16)                  -> non-NULL
```

### Constraints
- `arena` is 8-byte aligned; `size` is a multiple of 8 with `64 <= size <= 65536`
- No `malloc` and no static or global variables: two heaps must be fully independent
- Bookkeeping lives only in the arena and in `h->base` / `h->size`
- Python: bookkeeping lives only in the bytearray. Besides `h.arena`, `h` may hold only int fields (no lists, dicts or other containers, on `h` or in module globals), and the bytearray keeps its length

**Optimal complexity:** O(number of blocks) per `heap_alloc` and `heap_free`, no memory beyond the arena
