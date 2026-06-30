# Reverse and Rotate an Array

Implement in-place array reversal and left rotation without allocating any extra memory.

```c
void reverse_i32(int32_t *a, size_t n);
void rotate_left_i32(int32_t *a, size_t n, size_t k);
```

- **`reverse_i32`** — reverse the array in place using two pointers
- **`rotate_left_i32`** — rotate the array left by `k` positions; element at index `k` becomes the new first element. `k %= n` first (handle `k >= n`). Implement using three reversals

Rotate left by `k` using the three-reversal trick:
1. Reverse `a[0..k-1]`
2. Reverse `a[k..n-1]`
3. Reverse `a[0..n-1]`

## Examples

**Example 1** — reverse
```
a = {1,2,3,4,5}
reverse_i32(a, 5)  → {5,4,3,2,1}
reverse_i32(a, 0)  → unchanged
reverse_i32(a, 1)  → unchanged
```

**Example 2** — rotate left
```
a = {1,2,3,4,5}
rotate_left_i32(a, 5, 2)  → {3,4,5,1,2}
rotate_left_i32(a, 5, 0)  → unchanged
rotate_left_i32(a, 5, 5)  → unchanged  (k==n → no change)
```

**Example 3** — k > n (wraps)
```
a = {1,2,3}
rotate_left_i32(a, 3, 4)  → same as rotate by 1: {2,3,1}
```

## Constraints

- No extra allocation (`O(1)` extra space)
- `n == 0` or `n == 1` → do nothing
- `k %= n` before proceeding; if `k == 0` after modulo → do nothing

**Optimal complexity:** O(n)
