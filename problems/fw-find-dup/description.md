# Find a Duplicate in Range 0..n-1

Given an array of length `n` containing values in the range `[0, n-1]` where at least one value appears more than once, find any duplicate value. You may modify the array.

```c
bool find_duplicate_u32(uint32_t *a, size_t n, uint32_t *dup_out);
```

The function should return `true` and write the duplicate value to `*dup_out`.

Use cyclic sort: walk each element to its "home" index. When you find a value that's already at its home, that's a duplicate.

Algorithm: for each `i`, while `a[i] != i`:
- Let `v = a[i]`
- If `a[v] == v` → v is a duplicate; store it and return true
- Otherwise swap `a[i]` and `a[v]`

## Examples

**Example 1** — duplicate at start
```
a = {0, 1, 2, 0}   (n=4, duplicate=0)
find_duplicate_u32(a, 4, &dup) → true, dup=0
```

**Example 2** — duplicate in middle
```
a = {1, 3, 2, 3}   (n=4, duplicate=3)
find_duplicate_u32(a, 4, &dup) → true, dup=3
```

**Example 3** — n=2 smallest case
```
a = {0, 0}
find_duplicate_u32(a, 2, &dup) → true, dup=0
```

## Constraints

- Array has length `n`, values in `[0, n-1]`, at least one duplicate guaranteed
- May modify the array in place
- If multiple duplicates exist, returning any one is acceptable
- Values are `uint32_t`; treat them as indices directly (guaranteed `< n`)

**Optimal complexity:** O(n) — cyclic sort
