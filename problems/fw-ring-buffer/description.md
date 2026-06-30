# Ring Buffer with Full Flag

Implement a classic ring (circular) buffer for bytes. The buffer uses an explicit `full` flag to disambiguate between the full and empty states — both of which have `head == tail` when capacity is a power of two.

The `ringbuf_t` struct is provided; implement the five functions below.

```c
void   rb_init(ringbuf_t *rb, uint8_t *storage, size_t capacity);
bool   rb_push(ringbuf_t *rb, uint8_t byte);
bool   rb_pop(ringbuf_t *rb, uint8_t *out);
size_t rb_available(const ringbuf_t *rb);
size_t rb_free_space(const ringbuf_t *rb);
```

- **`rb_init`** — initialize the ring buffer with the given storage and capacity
- **`rb_push`** — add a byte; return `false` if full (do not overwrite)
- **`rb_pop`** — remove and return the oldest byte in `*out`; return `false` if empty
- **`rb_available`** — number of bytes currently stored
- **`rb_free_space`** — number of bytes that can still be pushed

## Examples

**Example 1** — basic push/pop
```
rb_init(&rb, buf, 4);
rb_push(&rb, 0xAA);  → true,  available=1
rb_push(&rb, 0xBB);  → true,  available=2
rb_pop(&rb, &v);     → true,  v=0xAA, available=1
```

**Example 2** — fill to capacity and overflow
```
rb_init(&rb, buf, 2);
rb_push(&rb, 1); rb_push(&rb, 2); → both true
rb_push(&rb, 3);                  → false (full)
```

**Example 3** — wraparound
```
rb_init(&rb, buf, 3);
rb_push(&rb, 10); rb_push(&rb, 20); rb_push(&rb, 30);
rb_pop(&rb, &v);  // v=10
rb_push(&rb, 40); // wraps head past end
rb_pop(&rb, &v);  // v=20
```

## Constraints

- The caller provides `storage`; the buffer does not allocate memory
- Empty: `head == tail && !full`; Full: `head == tail && full`
- `rb_available`: `full ? capacity : (tail - head + capacity) % capacity`
- `rb_free_space`: `capacity - rb_available(rb)`

**Optimal complexity:** O(1) per operation
