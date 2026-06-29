# Counting Bits

Given an integer `n`, return an array `ans` of length `n + 1` such that for each `i` (`0 <= i <= n`), `ans[i]` is the number of `1` bits in the binary representation of `i`.

```c
int* countBits(int n, int* returnSize);   // malloc'd array of length n+1
```

### Example 1
```
Input:  n = 2
Output: [0, 1, 1]      # 0->0, 1->1, 2->1
```

### Example 2
```
Input:  n = 5
Output: [0, 1, 1, 2, 1, 2]
```

### Constraints
- `0 <= n <= 10^5`
- Bonus: `ans[i] = ans[i >> 1] + (i & 1)` gives an O(n) one-pass DP.

**Optimal complexity:** O(n)  — each ans[i] derived from ans[i>>1] in O(1)
