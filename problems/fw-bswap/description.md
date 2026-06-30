# Byte Swap 16 and 32

Implement byte-swapping functions for 16-bit and 32-bit unsigned integers using only shifts and masks — no compiler builtins (`__builtin_bswap16`, `__builtin_bswap32`), no unions, and no `memcpy`.

Byte swapping is the fundamental operation for converting between big-endian and little-endian representations — essential when reading network packets, SPI sensor data, or any cross-architecture protocol.

```c
uint16_t bswap16_u(uint16_t x);
uint32_t bswap32_u(uint32_t x);
```

## Examples

**Example 1**
```
bswap16_u(0x1234) → 0x3412
bswap16_u(0x0100) → 0x0001
```

**Example 2**
```
bswap32_u(0x01020304) → 0x04030201
bswap32_u(0xAABBCCDD) → 0xDDCCBBAA
```

**Example 3**
```
bswap16_u(0x0000) → 0x0000
bswap16_u(0xFFFF) → 0xFFFF
bswap32_u(0x00000000) → 0x00000000
```

## Constraints

- Use only shifts (`<<`, `>>`) and masks (`&`, `|`) — no builtins
- `uint16_t` and `uint32_t` as defined in `<stdint.h>`

**Optimal complexity:** O(1) — fixed number of shifts
