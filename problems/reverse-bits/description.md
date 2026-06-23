# Reverse Bits

Reverse the bits of a given 32-bit unsigned integer.

```c
uint32_t reverseBits(uint32_t n);
```

### Example 1
```
Input:  n = 0000 0010 1001 1000 0001 1110 1001 1100   (43261596)
Output: 0011 1001 0111 1000 0001 0110 0100 1010        (964176192)
```

### Example 2
```
Input:  n = 1111 1111 1111 1111 1111 1111 1111 1101   (4294967293)
Output: 1011 1111 1111 1111 1111 1111 1111 1111        (3221225471)
```

### Constraints
- `0 <= n <= 2^32 - 1`
- Build the result bit by bit: shift the result left, OR in the lowest bit of `n`,
  then shift `n` right — 32 times.
