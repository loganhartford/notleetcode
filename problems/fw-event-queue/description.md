# Producer/Consumer Event Queue

Implement a FIFO event queue backed by a circular buffer. Each event carries a 16-bit ID and a 32-bit argument. Producers call `eq_push`; consumers call `eq_pop`.

The shared types `event_t` and `event_queue_t` are provided. Implement:

```c
void   eq_init(event_queue_t *q, event_t *storage, size_t capacity);
bool   eq_push(event_queue_t *q, event_t e);
bool   eq_pop(event_queue_t *q, event_t *out);
size_t eq_count(const event_queue_t *q);
```

- **`eq_push`** returns `false` when the queue is full; does not overwrite
- **`eq_pop`** returns `false` when empty
- Events are delivered in FIFO order
- `eq_count` returns the number of events currently queued

## Examples

**Example 1** — FIFO order
```
eq_init(&q, buf, 4);
eq_push(&q, {.id=1, .arg=100});
eq_push(&q, {.id=2, .arg=200});
eq_pop(&q, &e);  → true, e={1, 100}
eq_pop(&q, &e);  → true, e={2, 200}
```

**Example 2** — overflow fails
```
// Queue capacity 2
eq_push(...); eq_push(...);    // both succeed
eq_push(...);                  → false (full)
eq_count(&q)                   → 2
```

**Example 3** — wraparound
```
eq_init(&q, buf, 3);
push A, push B, push C; pop A; push D  // D wraps to index 0
pop → B, pop → C, pop → D
```

## Constraints

- The caller provides `storage`; no allocation
- `head` is the index of the next item to pop; `count` is items stored
- Write index: `(head + count) % capacity`
- FIFO: first pushed is first popped

**Optimal complexity:** O(1) per operation
