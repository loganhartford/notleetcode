# Implement a Circular Buffer

A **circular buffer** (ring buffer) is a fixed-size FIFO queue backed by a single array
whose ends "wrap around." It's the standard way firmware buffers a stream of items
between a producer and a consumer — e.g. bytes arriving from a UART interrupt while the
main loop drains them — without any dynamic allocation.

Implement that queue on top of the array below. Elements are added at the **back**
(`tail`) and removed from the **front** (`head`); when an index runs off the end of the
array it wraps back to index `0`. No heap, no shifting elements — just move the two
indices with modular arithmetic.

### What's provided

The struct, the capacity constant, and `cb_init` are already written for you:

```c
#define CB_CAPACITY 8

typedef struct {
    int  data[CB_CAPACITY];
    int  head;    /* index of the front element           */
    int  tail;    /* index where the next push will write */
    int  count;   /* current number of elements           */
} CircularBuffer;

void cb_init(CircularBuffer *cb);   /* sets head = tail = count = 0 */
```

`head == tail` happens both when the buffer is empty **and** when it is full, so you
can't tell those apart from the indices alone. The `count` field breaks the tie — track
it on every push/pop and all `CB_CAPACITY` (8) slots stay usable.

### Functions to implement

```c
int  cb_size    (const CircularBuffer *cb);   /* number of elements currently stored */
bool cb_is_empty(const CircularBuffer *cb);   /* true iff size == 0                   */
bool cb_is_full (const CircularBuffer *cb);   /* true iff size == CB_CAPACITY         */

/* Append value to the back. Returns 0 on success, -1 if the buffer is full. */
int  cb_push(CircularBuffer *cb, int value);

/* Remove the front element into *out. Returns 0 on success, -1 if empty. */
int  cb_pop (CircularBuffer *cb, int *out);

/* Copy the front element into *out without removing it. Returns 0, or -1 if empty. */
int  cb_peek(const CircularBuffer *cb, int *out);
```

### Behaviour

- **FIFO order:** the element popped is always the oldest one still in the buffer.
- `cb_push` on a **full** buffer returns `-1` and leaves the existing contents unchanged
  (it must **not** overwrite the oldest element).
- `cb_pop` and `cb_peek` on an **empty** buffer return `-1`; `*out` is written only when
  they return `0`.
- `cb_peek` does not change the buffer — size is the same before and after.
- After wrapping (interleaved pushes and pops that carry `head`/`tail` past the end of
  the array), FIFO order must still hold. The buffer is meant to be reused indefinitely.
- `cb_size`, `cb_is_empty`, and `cb_is_full` must be O(1) — no scanning the array.

### Example

```
cb_init(&cb);
cb_push(&cb, 10);  cb_push(&cb, 20);  cb_push(&cb, 30);   // size 3

cb_peek(&cb, &x);  // returns 0, x == 10, size still 3 (front, not removed)
cb_pop (&cb, &x);  // returns 0, x == 10
cb_pop (&cb, &x);  // returns 0, x == 20
cb_pop (&cb, &x);  // returns 0, x == 30
cb_is_empty(&cb);  // true
cb_pop (&cb, &x);  // returns -1 (empty)
```

Wrap-around: push `0..7` to fill it, pop a few from the front, then push more — the new
values reuse the freed slots at the start of the array while FIFO order is preserved.

### Constraints

- Capacity is exactly `CB_CAPACITY` = 8; a full buffer holds 8 elements.
- Every operation runs in O(1) time and O(1) extra space (no allocation, no memmove).

**Optimal complexity:** O(1) per operation using modular arithmetic on `head` and `tail`, with `count` tracking occupancy.
