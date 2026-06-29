# Sum of Two Integers

Given two integers `a` and `b`, return the sum of the two integers **without using the operators `+` and `-`**.

```c
int getSum(int a, int b);
```

### Example 1
```
Input:  a = 1, b = 2
Output: 3
```

### Example 2
```
Input:  a = 2, b = 3
Output: 5
```

### Example 3
```
Input:  a = -1, b = 1
Output: 0
```

### Constraints
- `-1000 <= a, b <= 1000`
- Use bit operations: `a ^ b` is the sum without carry; `(a & b) << 1` is the
  carry. Repeat until the carry is zero. (Do the carry math on `unsigned` to keep
  the shifts well-defined.)

**Optimal complexity:** O(1)  — at most 32 carry-propagation iterations
