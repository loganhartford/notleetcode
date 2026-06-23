# Is Power of Two, and Round Up to Next Power of Two

Implement two related bit tricks:

```c
bool     is_power_of_two(uint32_t x);   // is x exactly a power of two?
uint32_t round_up_pow2(uint32_t x);     // smallest power of two >= x
```

- `is_power_of_two(x)` returns `true` iff `x` has exactly one bit set. `0` is
  **not** a power of two.
- `round_up_pow2(x)` returns the smallest power of two that is `>= x`. By
  convention `round_up_pow2(0) == 1`, and if `x` is already a power of two it
  returns `x` unchanged.

### Examples
```
is_power_of_two(1)   -> true
is_power_of_two(16)  -> true
is_power_of_two(0)   -> false
is_power_of_two(6)   -> false

round_up_pow2(0)  -> 1
round_up_pow2(1)  -> 1
round_up_pow2(5)  -> 8
round_up_pow2(16) -> 16
round_up_pow2(17) -> 32
```

### Constraints
- `0 <= x <= 2^31` (results fit in `uint32_t`).
- `is_power_of_two`: `x && !(x & (x - 1))`.
- `round_up_pow2`: smear the highest set bit down with shifts/ORs, then `+1`.
- This is `[author]`-defined firmware practice.
