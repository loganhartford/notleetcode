# Power-of-Two Check

Implement a function that determines whether a 32-bit unsigned integer is a power of two.

Powers of two (1, 2, 4, 8, 16, …) have exactly one bit set in their binary representation. This property makes them particularly common in firmware for buffer sizes, alignment values, and bit masks.

```c
bool is_power_of_two_u32(uint32_t x);
```

Return `true` if `x` is a power of two, `false` otherwise. Note that 0 is **not** a power of two.

## Examples

**Example 1**
```
is_power_of_two_u32(1)  → true    // 2^0
is_power_of_two_u32(2)  → true    // 2^1
is_power_of_two_u32(4)  → true    // 2^2
```

**Example 2**
```
is_power_of_two_u32(0)  → false   // zero is not a power of two
is_power_of_two_u32(3)  → false   // binary 11 — two bits set
is_power_of_two_u32(6)  → false   // binary 110 — two bits set
```

**Example 3**
```
is_power_of_two_u32(0x80000000) → true    // 2^31
is_power_of_two_u32(0x80000001) → false   // two bits set
is_power_of_two_u32(32)         → true    // 2^5
```

## Constraints

- `x` is a `uint32_t` (0 to 4 294 967 295)
- 0 → `false`

**Optimal complexity:** O(1) — single AND operation
