# Queue from Two Stacks

Implement a FIFO queue using two fixed-size stacks. This classic data structures puzzle uses two arrays — `inbox` and `outbox` — to simulate FIFO ordering.

The `two_stack_queue_t` struct and `TSQ_CAP` constant are provided. Implement:

```c
bool tsq_push(two_stack_queue_t *q, int32_t v);
bool tsq_pop(two_stack_queue_t *q, int32_t *out);
```

**Push:** push `v` onto `inbox` (increment `in_top`). Return `false` if `inbox` is full (`in_top == TSQ_CAP`) and `outbox` is also full.

**Pop:**
1. If `outbox` is non-empty, pop from `outbox` (decrement `out_top`)
2. Otherwise, if `inbox` is non-empty, transfer all of `inbox` to `outbox` (this reverses order, giving FIFO from outbox), then pop
3. If both are empty, return `false`

## Examples

**Example 1** — FIFO order
```
tsq_push(&q, 1); tsq_push(&q, 2); tsq_push(&q, 3);
tsq_pop(&q, &v);  → v=1
tsq_pop(&q, &v);  → v=2
tsq_pop(&q, &v);  → v=3
```

**Example 2** — interleaved push/pop
```
tsq_push(&q, 10); tsq_push(&q, 20);
tsq_pop(&q, &v);   → v=10  (transfer: outbox=[20,10]→pop→10)
tsq_push(&q, 30);
tsq_pop(&q, &v);   → v=20
tsq_pop(&q, &v);   → v=30
```

**Example 3** — overflow and underflow
```
// Fill inbox fully, can't push more
tsq_push full inbox with outbox also full → false
tsq_pop on empty queue → false
```

## Constraints

- `TSQ_CAP = 16`; `in_top` and `out_top` start at 0
- Transfer: while `in_top > 0`, pop from `inbox[--in_top]` and push to `outbox[out_top++]`
- Push returns `false` only when `in_top == TSQ_CAP` (inbox full); if there's room in inbox, always push there — don't shuffle
- Push returns `false` if inbox is full (no room to accept new items)

**Optimal complexity:** O(1) amortized — each element is transferred at most once
