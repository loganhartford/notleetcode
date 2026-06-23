# Swap Endianness of a 32-bit Integer

Reverse the **byte order** of a 32-bit unsigned integer (convert between
little-endian and big-endian).

```c
uint32_t swap_endianness(uint32_t x);
```

Byte `[B3 B2 B1 B0]` becomes `[B0 B1 B2 B3]`.

### Example 1
```
Input:  0x12345678
Output: 0x78563412
```

### Example 2
```
Input:  0x000000FF
Output: 0xFF000000
```

### Example 3
```
Input:  0xAABBCCDD
Output: 0xDDCCBBAA
```

### Constraints
- Operate on all 4 bytes with shifts and masks (`0xFF`, `0xFF00`, …).
- This is `[author]`-defined firmware practice.
