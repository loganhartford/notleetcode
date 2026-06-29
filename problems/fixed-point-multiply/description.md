# Fixed-Point Multiply (Q-format)

Multiply two fixed-point numbers in **Qm.f** format, where `frac_bits` is the
number of fractional bits.

```c
int32_t qmul(int32_t a, int32_t b, int frac_bits);
```

A Q-format value stores a real number `r` as the integer `round(r * 2^frac_bits)`.
Multiplying two such integers gives a result scaled by `2^(2*frac_bits)`, so you
must shift back down by `frac_bits`.

The catch: `a * b` can overflow 32 bits, so do the multiply in **64-bit** and
then shift:

```
return (int32_t)(((int64_t)a * (int64_t)b) >> frac_bits);
```

(Truncating shift — round toward negative infinity — is fine here.)

### Example (Q16.16, frac_bits = 16)
```
1.5  -> 0x00018000   (98304)
2.0  -> 0x00020000   (131072)
qmul(98304, 131072, 16) -> 196608  (0x00030000 = 3.0)
```

### Constraints
- `0 <= frac_bits <= 31`
- Inputs and the true product fit in the obvious ranges; use `int64_t` for the
  intermediate.
- `[author]`-defined firmware practice.

**Optimal complexity:** O(1)  — single 64-bit multiply and right-shift
