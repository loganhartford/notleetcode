# Min Stack

Design a stack that supports push, pop, top, and retrieving the minimum element — all in **O(1)** time.

Implement the `MinStack` class:

- `MinStack()` — initializes the stack.
- `push(val)` — pushes `val` onto the stack.
- `pop()` — removes the element on top.
- `top()` — returns the top element.
- `getMin()` — returns the minimum element currently in the stack.

Methods are always called validly (no `pop`/`top`/`getMin` on an empty stack).

### Example
```
MinStack()        ->  null
push(-2)          ->  null
push(0)           ->  null
push(-3)          ->  null
getMin()          ->  -3
pop()             ->  null
top()             ->  0
getMin()          ->  -2
```

### Constraints
- `-2^31 <= val <= 2^31 - 1`
- At most `3 * 10^4` calls total.

The hidden tests drive a sequence of operations and compare the returned values
(`null` for the void operations).
