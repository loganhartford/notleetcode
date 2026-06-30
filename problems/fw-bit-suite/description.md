# Popcount, Bit Reverse, and Rotate

Implement four bit-manipulation utility functions that appear frequently in low-level firmware and DSP code. Do not use compiler builtins (`__builtin_popcount`, `__builtin_bswap`, etc.).

```c
unsigned popcount32(uint32_t x);
uint32_t reverse_bits32(uint32_t x);
uint32_t rotl32(uint32_t x, unsigned n);
uint32_t rotr32(uint32_t x, unsigned n);
```

- **`popcount32`** — count the number of 1-bits (Hamming weight)
- **`reverse_bits32`** — reverse the bit order (MSB ↔ LSB)
- **`rotl32`** — rotate left by `n` bits (wrap n modulo 32; if n==0, return x unchanged)
- **`rotr32`** — rotate right by `n` bits (wrap n modulo 32; if n==0, return x unchanged)

## Examples

**Example 1** — popcount
```
popcount32(0x00000000) → 0
popcount32(0xFFFFFFFF) → 32
popcount32(0x55555555) → 16   // alternating bits
popcount32(0x00000007) → 3
```

**Example 2** — reverse bits
```
reverse_bits32(0x80000000) → 0x00000001
reverse_bits32(0x00000001) → 0x80000000
reverse_bits32(0xFFFFFFFF) → 0xFFFFFFFF
```

**Example 3** — rotate
```
rotl32(0x00000001, 1)  → 0x00000002
rotl32(0x80000000, 1)  → 0x00000001   // wraps MSB to LSB
rotr32(0x00000001, 1)  → 0x80000000   // wraps LSB to MSB
rotl32(x, 0)           → x            // n=0 no-op
rotl32(x, 32)          → x            // n=32 same as n=0
```

## Constraints

- No compiler builtins
- For `rotl32`/`rotr32`: reduce `n %= 32` first; if `n == 0`, return `x` unchanged (avoids undefined shift by 32)
- `reverse_bits32`: may use a 32-iteration loop

**Optimal complexity:** O(k) popcount where k = set bit count; O(1) for rotate/reverse
