# Q15 Saturating Arithmetic

Implement add, subtract and multiply for Q15 fixed-point numbers, the 16-bit fractional format used throughout DSP code. A Q15 value is an `int16_t` `x` that represents the real number `x / 32768`. The representable range is `-1.0` (`-32768`, bit pattern `0x8000`) up to `1 - 2^-15` (`32767`, `0x7FFF`). For example, `0x4000` is `0.5`, `0x2000` is `0.25`, and `-16384` (`0xC000`) is `-0.5`.

```c
typedef int16_t q15_t;

q15_t q15_add_sat(q15_t a, q15_t b);
q15_t q15_sub_sat(q15_t a, q15_t b);
q15_t q15_mul(q15_t a, q15_t b);
```

- **`q15_add_sat(a, b)`** returns the exact sum `a + b`, saturated: a sum above `32767` returns `32767`, and a sum below `-32768` returns `-32768`.
- **`q15_sub_sat(a, b)`** returns the exact difference `a - b`, saturated the same way.
- **`q15_mul(a, b)`** returns the real product `(a / 32768) * (b / 32768)` as a Q15 value. In integer terms, the result is the integer nearest to `a * b / 32768`; when that quotient lies exactly halfway between two integers, return the larger one (round half toward +infinity). The rounded result is then saturated to `[-32768, 32767]` like the other two.

Python: implement the same three functions, `q15_add_sat(a: int, b: int) -> int`, `q15_sub_sat(a: int, b: int) -> int` and `q15_mul(a: int, b: int) -> int`, on plain ints in `[-32768, 32767]`, with the same results.

### Example 1
```
Input:  q15_add_sat(0x4000, 0x2000)     0.5 + 0.25
Output: 0x6000                          0.75
```

### Example 2
```
Input:  q15_add_sat(0x7000, 0x2000)     0.875 + 0.25 = 1.125, out of range
Output: 0x7FFF                          saturated to the largest value
```

### Example 3
```
Input:  q15_mul(0x4000, 0x4000)         0.5 * 0.5
Output: 0x2000                          0.25
```

### Example 4
```
Input:  q15_mul(1, 0x4000)              a * b / 32768 = 0.5, exactly halfway
Output: 1                               ties go toward +infinity
```

### Example 5
```
Input:  q15_mul(-1, 0x4000)             a * b / 32768 = -0.5, exactly halfway
Output: 0                               ties go toward +infinity
```

### Constraints
- `a` and `b` can be any `int16_t` value (Python: any int in `[-32768, 32767]`)
- The result is always in `[-32768, 32767]`
- No floating point
- No undefined behavior for any pair of inputs

**Optimal complexity:** O(1) time and space per call
