# Implement Queue using Stacks

Implement a first-in-first-out (FIFO) **queue** using only two **stacks**. The implemented queue should support all the normal functions (`push`, `peek`, `pop`, `empty`).

Implement the `MyQueue` class:

- `MyQueue()` — initializes the queue.
- `push(x)` — pushes element `x` to the back of the queue.
- `pop()` — removes the element from the front of the queue and returns it.
- `peek()` — returns the element at the front of the queue.
- `empty()` — returns `True` if the queue is empty, `False` otherwise.

You must use only standard stack operations: push to top, peek/pop from top, size, and is-empty.

### Example
```
MyQueue()     ->  null
push(1)       ->  null
push(2)       ->  null
peek()        ->  1
pop()         ->  1
empty()       ->  False
```

### Constraints
- `1 <= x <= 9`
- At most `100` calls to `push`, `pop`, `peek`, and `empty`.
- `pop` and `peek` are always called on a non-empty queue.

The hidden tests drive a sequence of operations and compare the returned values
(`null` for the void operations).

**Optimal complexity:** O(1) amortized per operation  — lazy transfer between stacks
