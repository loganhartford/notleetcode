# Implement a Circular Buffer

Implement a fixed-capacity FIFO queue backed by a stack-allocated array.

The `CircularBuffer` struct, the `CB_CAPACITY` constant, and `cb_init` are already provided.
Implement the remaining six functions.

### Struct

```c
#define CB_CAPACITY 8

typedef struct {
    int  data[CB_CAPACITY];
    int  head;    /* index of the front element           */
    int  tail;    /* index where the next push will write */
    int  count;   /* current number of elements           */
} CircularBuffer;
```

### Functions to implement

```c
int  cb_size   (const CircularBuffer *cb);
bool cb_is_empty(const CircularBuffer *cb);
bool cb_is_full (const CircularBuffer *cb);

/* Append value to the back. Returns 0 on success, -1 if full. */
int  cb_push(CircularBuffer *cb, int value);

/* Remove front element into *out. Returns 0 on success, -1 if empty. */
int  cb_pop (CircularBuffer *cb, int *out);

/* Read front element into *out without removing. Returns 0, or -1 if empty. */
int  cb_peek(const CircularBuffer *cb, int *out);
```

### Behaviour

- `cb_push` appends to the back; `cb_pop` removes from the front (FIFO).
- When full, `cb_push` returns `-1` and does **not** overwrite existing data.
- `cb_pop` and `cb_peek` return `-1` when the buffer is empty.
- `cb_size`, `cb_is_empty`, `cb_is_full` must be O(1).
- Indices must wrap around correctly — the buffer must be reusable after interleaved pushes and pops.

### Example

```
cb_init(&cb);
cb_push(&cb, 10);  cb_push(&cb, 20);  cb_push(&cb, 30);

cb_peek(&cb, &x);  // x == 10, size unchanged
cb_pop (&cb, &x);  // x == 10
cb_pop (&cb, &x);  // x == 20
cb_pop (&cb, &x);  // x == 30
cb_is_empty(&cb);  // true
```

### Constraints

- Capacity is exactly `CB_CAPACITY` = 8.
- All operations must run in O(1) time and O(1) extra space.

**Optimal complexity:** O(1) per operation using modular arithmetic on `head` and `tail`
