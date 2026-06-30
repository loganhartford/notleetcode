# Subarray Sum — Non-Negative Sliding Window

Given an array of non-negative `uint32_t` values and a target sum, find a contiguous subarray whose elements sum to exactly `target`. Return the start (inclusive) and end (exclusive) indices.

```c
bool subarray_sum_nonneg(const uint32_t *a, size_t n, uint32_t target,
                          size_t *start_out, size_t *end_out);
```

Because all elements are non-negative, a sliding window approach works: expand the window by moving `end` right; shrink by moving `start` right when the sum exceeds the target.

Special case: if `target == 0`, return `true` with `*start_out = 0, *end_out = 0` (empty subarray).

## Examples

**Example 1** — found at start
```
a = {1,2,3,4,5},  target=6
start=1, end=3  (a[1]+a[2] = 2+3+... wait: 1+2+3=6: start=0,end=3)
```

**Example 2** — found in middle
```
a = {1,2,3,4,5},  target=9
a[1]+a[2]+a[3] = 2+3+4=9 → start=1, end=4
```

**Example 3** — not found
```
a = {1,2,3},  target=7  → false
```

## Constraints

- All elements are non-negative (`uint32_t`)
- `*end_out` is the exclusive end index (one past the last element)
- `target == 0` → return `true`, `start=0`, `end=0` (empty subarray, sum=0)
- Single element equal to target: `start=i`, `end=i+1`
- Return `false` if no subarray sums to `target`

**Optimal complexity:** O(n) — sliding window
