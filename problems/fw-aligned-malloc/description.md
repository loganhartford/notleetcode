# Implement aligned_malloc

Implement `aligned_malloc(size, align)` and `aligned_free(ptr)` using only the standard `malloc` / `free`.

## Signatures

```c
void *aligned_malloc(size_t size, size_t align);
void  aligned_free(void *ptr);
```

## Behaviour

- `align` is always a power of two (e.g. 4, 8, 16, 64).
- The returned pointer must satisfy `(uintptr_t)ptr % align == 0`.
- `aligned_free` must correctly free the block without leaking memory regardless of how much the pointer was adjusted.
- Return `NULL` if `malloc` fails or if `align` is 0.

## Hint

Allocate `size + align + sizeof(void*)` bytes. Store the original `malloc` pointer just before the aligned region so `aligned_free` can retrieve it.

## Examples

```
aligned_malloc(100, 16)  → pointer with (ptr % 16) == 0
aligned_malloc(1,   64)  → pointer with (ptr % 64) == 0
aligned_free(ptr)        → frees without leaking
```

## Constraints

- Only `malloc` / `free` from `<stdlib.h>` are allowed — no `posix_memalign`, `memalign`, etc.
- `align` is a power of two, `1 ≤ align ≤ 4096`.
- `size` is at least 1.
