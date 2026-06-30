# Fixed-Size Stack and Queue

Implement a LIFO stack and a FIFO queue, both backed by fixed-size arrays. No dynamic allocation.

The `fwstack_t` and `queue32_t` types are provided. Implement:

```c
bool   stack_push(fwstack_t *s, int32_t v);
bool   stack_pop(fwstack_t *s, int32_t *out);
bool   q32_push(queue32_t *q, int32_t v);
bool   q32_pop(queue32_t *q, int32_t *out);
size_t q32_count(const queue32_t *q);
```

**Stack** (`fwstack_t`): LIFO. `buf` is caller-provided, `cap` is its capacity, `count` is current depth. Push appends to `buf[count]`; pop reads from `buf[count-1]`. Both return `false` on over/underflow.

**Queue** (`queue32_t`): FIFO circular buffer with compile-time capacity `FW_QUEUE_CAP`. `head` is the read index, `count` is items stored. Write index = `(head + count) % FW_QUEUE_CAP`.

## Examples

**Example 1** — stack LIFO
```
stack_push(&s, 1); stack_push(&s, 2); stack_push(&s, 3);
stack_pop(&s, &v); → true, v=3
stack_pop(&s, &v); → true, v=2
```

**Example 2** — queue FIFO
```
q32_push(&q, 10); q32_push(&q, 20); q32_push(&q, 30);
q32_pop(&q, &v);  → true, v=10
q32_pop(&q, &v);  → true, v=20
```

**Example 3** — overflow / underflow
```
// stack full:
stack_push full → false
// queue empty:
q32_pop empty   → false
```

## Constraints

- Stack: caller provides `buf` and `cap`; the struct must be initialized with `count = 0`
- Queue: `FW_QUEUE_CAP` = 16; queue wraps around using modulo
- Both return `false` on overflow (full) and underflow (empty)

**Optimal complexity:** O(1) per operation
