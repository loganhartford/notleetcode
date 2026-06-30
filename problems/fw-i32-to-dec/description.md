# Integer to Decimal String

Implement a function that converts a 32-bit signed integer to a null-terminated decimal string. Do not use `sprintf`, `itoa`, or any other formatting functions.

```c
bool i32_to_dec(char *out, size_t out_len, int32_t value);
```

Return `true` on success (string fits in buffer with null terminator). Return `false` if `out_len` is too small.

`INT32_MIN` (-2147483648) is the trickiest case: negating it overflows `int32_t`. Handle it correctly.

## Examples

**Example 1** — basic values
```
i32_to_dec(buf, 16, 0)    → true, buf="0"
i32_to_dec(buf, 16, 1)    → true, buf="1"
i32_to_dec(buf, 16, -1)   → true, buf="-1"
i32_to_dec(buf, 16, 42)   → true, buf="42"
```

**Example 2** — extremes
```
i32_to_dec(buf, 12, 2147483647)  → true, buf="2147483647"
i32_to_dec(buf, 12, -2147483648) → true, buf="-2147483648"
```

**Example 3** — buffer sizing
```
i32_to_dec(buf, 3, 42)   → true, buf="42"     (needs 3 bytes: '4','2','\0')
i32_to_dec(buf, 2, 42)   → false              (too small)
i32_to_dec(buf, 2, 0)    → true, buf="0"      (needs 2 bytes: '0','\0')
i32_to_dec(buf, 1, 0)    → false              (too small for "0\0")
i32_to_dec(buf, 0, 0)    → false
```

## Constraints

- No `sprintf`, `snprintf`, `itoa`, or `printf`
- `INT32_MIN` must be handled without undefined behavior (use `uint32_t` for digit extraction)
- Return `false` if buffer (including space for `'\0'`) is too small

**Optimal complexity:** O(log n) — one digit per division
