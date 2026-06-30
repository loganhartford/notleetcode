# Binary Search: Lower Bracket Index

Given a sorted array of x-values and a target, find the largest index `i` such that `xs[i] <= target`. This is the "lower bracket" needed before interpolation: you know the segment starts at index `i`.

```c
bool lower_bracket_index(const int32_t *xs, size_t n,
                          int32_t target, size_t *idx_out);
```

Return `false` if:
- `n == 0`
- `target < xs[0]` (below the lowest value)

Return the largest `i` where `xs[i] <= target`. If `target >= xs[n-1]`, return `n-1`.

## Examples

**Example 1** — exact match
```
xs = {10, 20, 30, 40}
lower_bracket_index(xs, 4, 20, &i) → true, i=1
lower_bracket_index(xs, 4, 10, &i) → true, i=0
lower_bracket_index(xs, 4, 40, &i) → true, i=3
```

**Example 2** — between two values
```
xs = {10, 20, 30, 40}
lower_bracket_index(xs, 4, 25, &i) → true, i=1  (xs[1]=20 <= 25 < xs[2]=30)
lower_bracket_index(xs, 4, 39, &i) → true, i=2
```

**Example 3** — out of range
```
lower_bracket_index(xs, 4, 9,  &i) → false  (below xs[0])
lower_bracket_index(xs, 0, 10, &i) → false  (empty array)
lower_bracket_index(xs, 4, 99, &i) → true, i=3  (above xs[3], returns last index)
```

## Constraints

- `xs` is sorted ascending
- `n == 0` → `false`
- `target < xs[0]` → `false`
- `target >= xs[n-1]` → `i = n-1`

**Optimal complexity:** O(log n) — binary search
