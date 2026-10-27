# Bitmap ID Allocator

Implement an allocator that hands out integer IDs `0..BM_IDS-1` (DMA channels, mailbox slots, handles) and tracks which are in use with one bit per ID. All of its state lives in the `bitmap_t` the caller passes in: no `malloc`, no static or global storage. ID `i` is in use exactly when bit `i % 32` of `bits[i / 32]` is 1, where bit `k` of a word is the bit with value `1u << k`. This layout is part of the contract, and the raw words are checked after every call.

```c
#define BM_IDS 128

typedef struct {
    uint32_t bits[BM_IDS / 32];
} bitmap_t;

void bm_init(bitmap_t *b);
int  bm_alloc(bitmap_t *b);
int  bm_alloc_run(bitmap_t *b, unsigned n);
bool bm_free(bitmap_t *b, int id);
```

- **`bm_init`** marks every ID free. `b` may contain garbage, or be a bitmap that was already in use.
- **`bm_alloc`** marks the lowest free ID in use and returns it. If every ID is in use it returns `-1`.
- **`bm_alloc_run`** finds the lowest `s` such that IDs `s` through `s + n - 1` are all free, marks those `n` IDs in use, and returns `s`. A run may span more than one word of `bits`. It returns `-1` if no such run exists, if `n == 0`, or if `n > BM_IDS`; whenever it returns `-1`, `b` is unchanged.
- **`bm_free`** marks `id` free and returns `true` if `id` was in use. If `id` is outside `0..BM_IDS-1` or is already free, it returns `false` and changes nothing.

Python twin: the same four functions take a `Bitmap` object (defined in the starter) whose `bits` list holds `BM_IDS // 32` = 4 ints with the same layout; `bm_free(b, id)` returns `True` or `False`.

### Example 1
```
bm_init(&b);
bm_alloc(&b)          -> 0
bm_alloc(&b)          -> 1
bm_alloc(&b)          -> 2      bits = {0x00000007, 0, 0, 0}
bm_free(&b, 1)        -> true   bits = {0x00000005, 0, 0, 0}
bm_free(&b, 1)        -> false  (already free)
bm_alloc(&b)          -> 1
```

### Example 2
```
IDs 0..29 in use                bits = {0x3FFFFFFF, 0, 0, 0}
bm_alloc_run(&b, 4)   -> 30     bits = {0xFFFFFFFF, 0x00000003, 0, 0}
bm_alloc_run(&b, 0)   -> -1
bm_alloc_run(&b, 129) -> -1
bm_free(&b, 128)      -> false
```

### Example 3
```
bm_init(&b);
bm_alloc_run(&b, 128) -> 0      (every ID is now in use)
bm_alloc(&b)          -> -1
free IDs 10..14 and 40..41      (each bm_free -> true)
bm_alloc_run(&b, 2)   -> 10     (lowest start, although 40..41 fits exactly)
bm_alloc_run(&b, 3)   -> 12
bm_alloc_run(&b, 3)   -> -1     (only 40..41 are free; b unchanged)
bm_alloc(&b)          -> 40
```

### Constraints
- `BM_IDS` = 128
- `n` may be any `unsigned`, including `0` and values above `BM_IDS`; `id` may be any `int`, including negative values
- No `malloc` and no static or global variables: two `bitmap_t`s must be fully independent
- Compiler builtins such as `__builtin_ctz` are allowed but not required
- Python: `bits` stays a list of 4 ints, each in `[0, 2^32)`, and the `Bitmap` has no other attributes; no sets, dicts, or lists of bools as storage

**Optimal complexity:** O(BM_IDS) worst case per call; O(1) extra space
