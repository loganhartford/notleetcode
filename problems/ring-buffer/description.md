# Implement a Ring Buffer (FIFO byte queue)

Implement a fixed-capacity **ring buffer** over a caller-provided byte array — the
classic firmware FIFO. The storage does **not** belong to the ring buffer; the
caller passes it in, so the structure does no allocation.

You are given this type and must implement the functions:

```c
typedef struct {
    uint8_t* buf;     // caller-owned storage
    size_t   cap;     // capacity in bytes
    size_t   head;    // index of the oldest byte
    size_t   count;   // number of bytes currently stored
} ring_buffer_t;

void   rb_init(ring_buffer_t* rb, uint8_t* storage, size_t capacity);
bool   rb_push(ring_buffer_t* rb, uint8_t byte);  // false if full (byte dropped)
bool   rb_pop(ring_buffer_t* rb, uint8_t* out);   // false if empty (*out untouched)
size_t rb_count(const ring_buffer_t* rb);
bool   rb_is_empty(const ring_buffer_t* rb);
bool   rb_is_full(const ring_buffer_t* rb);
```

### Behavior
- `rb_init` binds the storage and resets the buffer to empty.
- `rb_push` appends one byte to the back; returns `false` (without writing) if full.
- `rb_pop` removes the oldest byte into `*out`; returns `false` if empty.
- Indices wrap around modulo `cap`. Use a separate `count` (or a one-slot-empty
  scheme) to distinguish **full** from **empty** — the bug that bites everyone.

### Example
```
rb_init(cap=4)
rb_is_empty()        -> true
rb_push(10,20,30,40) -> all true
rb_push(50)          -> false   (full)
rb_count()           -> 4
rb_pop() x2          -> 10, 20
rb_push(50), (60)    -> true, true   (reuse the freed slots — wrap-around)
rb_pop() x4          -> 30, 40, 50, 60
rb_is_empty()        -> true
```

### Notes
- `1 <= cap <= 4096`. If the capacity is a power of two you *can* mask instead of
  using `%`, but it isn't required here.

**Optimal complexity:** O(1) per operation
