# Q16.16 Fixed-Point Multiply

Implement fixed-point arithmetic in Q16.16 format — 16 integer bits and 16 fractional bits, represented as a signed 32-bit integer. The value 1.0 is stored as 65536 (0x00010000).

```c
typedef int32_t q16_t;

q16_t   q16_mul(q16_t a, q16_t b);
q16_t   q16_from_int(int32_t x);
int32_t q16_to_int(q16_t x);
```

- **`q16_from_int(x)`** — convert integer to Q16.16: `x << 16`
- **`q16_to_int(x)`** — convert Q16.16 to integer (truncate toward zero): arithmetic right shift by 16
- **`q16_mul(a, b)`** — multiply two Q16.16 values: `(int64_t)a * b >> 16`

## Examples

**Example 1** — basic multiplication
```
q16_t one  = q16_from_int(1);   // 65536
q16_t two  = q16_from_int(2);   // 131072
q16_mul(one, two) == q16_from_int(2)   // 1.0 * 2.0 = 2.0
```

**Example 2** — fractional values
```
q16_t half = 32768;             // 0.5 in Q16.16
q16_mul(half, half) == 16384   // 0.5 * 0.5 = 0.25
```

**Example 3** — conversion
```
q16_from_int(3)       → 196608  // 3 * 65536
q16_to_int(196608)    → 3
q16_to_int(32768)     → 0       // 0.5 truncates to 0
```

## Constraints

- Use `int64_t` intermediate for `q16_mul` to avoid overflow
- `q16_to_int`: arithmetic right shift (implementation-defined for negative in C, but works on all common targets; alternatively use `/65536`)
- Representable range: approximately -32768.0 to +32767.99998

**Optimal complexity:** O(1)
