# Hamming Distance

Return the number of bit positions where two integers differ.

```c
int hamming_distance(int a, int b);
```

### Examples

```
hamming_distance(5, 6)  → 2    // 0b101 ^ 0b110 = 0b011 → 2 bits set
hamming_distance(0, 0)  → 0
hamming_distance(1, 2)  → 2    // 0b01 ^ 0b10 = 0b11
```

### The obvious approach

XOR the two values, then test every bit position with a shift loop:

```c
unsigned int diff = (unsigned int)a ^ (unsigned int)b;
for (int i = 0; i < 32; i++) count += (diff >> i) & 1;
```

This always iterates 32 times regardless of how many bits are set.

### The optimal approach

Brian Kernighan's trick: `n &= n - 1` clears the lowest set bit in one operation. Loop only runs *k* times, where *k* is the number of set bits:

```c
while (diff) { diff &= diff - 1; count++; }
```

**Why it works:** subtracting 1 from any integer flips the lowest set bit to 0 and all trailing zeros to 1. ANDing with the original clears exactly that lowest set bit and leaves higher bits unchanged.

### Implementation note

Use `unsigned int` for the XOR result. Right-shifting a negative `int` is implementation-defined in C — casting to `unsigned` first makes the behavior defined on all platforms.

**Optimal complexity:** O(k) where k = number of differing bits
