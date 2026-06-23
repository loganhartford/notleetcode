# Number of 1 Bits (popcount)

Write a function that takes a 32-bit unsigned integer and returns the number of `1` bits it has (also known as the **Hamming weight** / popcount).

```c
int hammingWeight(uint32_t n);
```

### Example 1
```
Input:  n = 11   (binary 0000...0000 1011)
Output: 3
```

### Example 2
```
Input:  n = 128  (binary 0000...1000 0000)
Output: 1
```

### Example 3
```
Input:  n = 4294967293   (binary 1111...1111 1101)
Output: 31
```

### Constraints
- `0 <= n <= 2^32 - 1`
- Bonus: the `n &= n - 1` trick clears the lowest set bit each iteration, so the
  loop runs once per set bit.
