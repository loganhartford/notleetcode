# Moving Average from Data Stream

Design a `MovingAverage` class that computes the rolling average of the last `k` values in a stream of integers.

```cpp
class MovingAverage {
public:
    MovingAverage(int k);   // window size
    double next(int value); // add value; return average of last k values
};
```

The window slides as new values arrive. If fewer than `k` values have been seen, the average is over all values seen so far.

## Example

```
MovingAverage ma(3);
ma.next(1)  → 1.0         // window: [1]
ma.next(10) → 5.5         // window: [1, 10]
ma.next(3)  → 4.666...    // window: [1, 10, 3]
ma.next(5)  → 6.0         // window: [10, 3, 5]  (1 dropped)
```

## Constraints

- `1 <= k <= 1000`
- `-2^31 <= value <= 2^31 - 1`
- At most 10,000 calls to `next()`

**Optimal complexity:** O(1) per call — use a circular buffer or deque to maintain the window sum.
