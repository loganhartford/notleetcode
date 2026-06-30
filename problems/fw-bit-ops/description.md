# Set, Clear, Toggle, and Test a Bit

Implement four fundamental bit-manipulation functions that operate on a single bit position within a 32-bit unsigned integer.

These operations appear everywhere in firmware: configuring peripheral registers, managing flag fields, and toggling GPIO pins.

- **`bit_set(x, n)`** — return `x` with bit `n` set to 1
- **`bit_clear(x, n)`** — return `x` with bit `n` cleared to 0
- **`bit_toggle(x, n)`** — return `x` with bit `n` flipped
- **`bit_test(x, n)`** — return `true` if bit `n` is 1, `false` otherwise

Bit positions are zero-indexed from the LSB. If `n >= 32`, return the original value unchanged (for set/clear/toggle) or `false` (for test).

## Examples

**Example 1**
```
bit_set(0x00000000, 0)   → 0x00000001
bit_set(0x00000001, 7)   → 0x00000081
bit_test(0x00000081, 7)  → true
```

**Example 2**
```
bit_clear(0x000000FF, 3) → 0x000000F7
bit_toggle(0x000000F7, 3) → 0x000000FF
```

**Example 3**
```
bit_set(0xFFFFFFFF, 31)   → 0xFFFFFFFF   (already set)
bit_clear(0x00000000, 31) → 0x00000000   (already clear)
bit_toggle(0x00000001, 0) → 0x00000000
```

## Constraints

- `0 <= n`; if `n >= 32`, return `x` unchanged (set/clear/toggle) or `false` (test)
- Use `UINT32_C(1) << n` to avoid undefined behavior

**Optimal complexity:** O(1) — single shift and bitwise op
