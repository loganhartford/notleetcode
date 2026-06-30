# Register Bitfield Helpers

Implement four helper functions for safe read-modify-write operations on hardware registers. These patterns appear constantly in peripheral drivers where a mask specifies which bits a field occupies.

```c
uint32_t reg_set_bits(uint32_t reg, uint32_t mask);
uint32_t reg_clear_bits(uint32_t reg, uint32_t mask);
uint32_t reg_write_field(uint32_t reg, uint32_t mask, unsigned shift, uint32_t value);
uint32_t reg_read_field(uint32_t reg, uint32_t mask, unsigned shift);
```

- **`reg_set_bits`** — OR the mask into the register
- **`reg_clear_bits`** — AND the inverse of the mask into the register
- **`reg_write_field`** — clear masked bits, then insert `value` shifted into position; only the bits of `value` that fit within `mask >> shift` are used
- **`reg_read_field`** — extract the masked bits and shift them down

## Examples

**Example 1** — set/clear at LSB
```
reg_set_bits(0x00000000, 0x00000001)   → 0x00000001
reg_clear_bits(0x000000FF, 0x000000FF) → 0x00000000
```

**Example 2** — write a field
```
// mask=0x0000FF00, shift=8, value=0xAB
reg_write_field(0xFFFF0000, 0x0000FF00, 8, 0xAB) → 0xFFFFAB00
```

**Example 3** — read a field
```
reg_read_field(0x0000AB00, 0x0000FF00, 8) → 0x000000AB
```

## Constraints

- All values are `uint32_t`
- `mask` indicates which bits belong to the field; `shift` is the LSB position of the field
- `reg_write_field` must preserve bits outside the mask

**Optimal complexity:** O(1) — single mask and shift operation
