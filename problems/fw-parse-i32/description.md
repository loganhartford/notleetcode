# Parse Integer with Overflow Detection

Implement a safe integer parser that converts a decimal string to a 32-bit signed integer. Unlike `atoi`, this function detects overflow and invalid input.

```c
bool parse_i32(const char *s, int32_t *out);
```

Return `true` and write `*out` if the string represents a valid `int32_t` value. Return `false` for any invalid input.

Valid input format:
- Optional leading `+` or `-`
- One or more decimal digits
- Nothing else (no leading whitespace, no trailing characters)

## Examples

**Example 1** — valid inputs
```
parse_i32("0",     &v) → true,  v=0
parse_i32("123",   &v) → true,  v=123
parse_i32("-456",  &v) → true,  v=-456
parse_i32("+789",  &v) → true,  v=789
```

**Example 2** — boundary values
```
parse_i32("2147483647",  &v) → true,  v=INT32_MAX
parse_i32("-2147483648", &v) → true,  v=INT32_MIN
parse_i32("2147483648",  &v) → false  (overflow)
parse_i32("-2147483649", &v) → false  (underflow)
```

**Example 3** — invalid inputs
```
parse_i32("",    &v) → false  (empty)
parse_i32("12x", &v) → false  (non-digit)
parse_i32("  5", &v) → false  (leading whitespace)
parse_i32("+",   &v) → false  (sign only, no digits)
```

## Constraints

- No `atoi`, `strtol`, `sscanf`, or other conversion functions
- Detect overflow before it happens (compare against `INT32_MAX / 10` etc.)
- Empty string → `false`; sign with no digits → `false`

**Optimal complexity:** O(n)
