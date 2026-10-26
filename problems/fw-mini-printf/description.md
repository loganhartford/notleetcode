# Minimal snprintf

Implement a subset of C99 `snprintf`: format `fmt` and the arguments that follow it into `buf`, a buffer of `cap` bytes, without calling any formatting function from the C library.

```c
int mini_snprintf(char *buf, size_t cap, const char *fmt, ...);
```

Every character of `fmt` that is not part of a conversion is copied to the output unchanged. The supported conversions:

| Spec | Argument | Output |
|------|----------|--------|
| `%d` | `int` | signed decimal |
| `%u` | `unsigned int` | unsigned decimal |
| `%x` | `unsigned int` | unsigned hexadecimal, lowercase `a`–`f`, no `0x` prefix |
| `%c` | `int` | one character, the argument converted to `unsigned char` |
| `%s` | `const char *` | the string up to its NUL; a `NULL` pointer prints `(null)` |
| `%%` | none | a literal `%` |

`%d`, `%u` and `%x` (and only those) accept an optional `0` flag followed by an optional single-digit minimum field width `1`–`9`: `%4d`, `%08x`, `%02u`. A number shorter than the width is padded on the left to the width, with spaces, or with zeros when the `0` flag is given. A zero-padded negative number keeps its `-` in front: `%05d` of `-42` is `-0042`. A number as wide as the width or wider is printed in full.

Return the number of characters the complete output has, not counting the terminator, whatever `cap` is. If `cap > 0`, write at most the first `cap - 1` characters of the output followed by `'\0'`, so `buf` always ends up holding a NUL-terminated string. If `cap == 0`, write nothing; `buf` may be `NULL`. A return value `>= cap` tells the caller the output was truncated.

Python: `mini_snprintf(buf: bytearray, fmt: bytes, *args) -> int`, where `cap` is `len(buf)`; `%d`/`%u`/`%x` take ints already in C `int`/`unsigned int` range, `%c` takes a 1-byte `bytes` or an int, `%s` takes `bytes` or `None`, and `buf` must not change length.

### Example 1
```
Input:  cap = 32, fmt = "id=%u temp=%d", 7, -15
Output: buf = "id=7 temp=-15", returns 13
```

### Example 2
```
Input:  cap = 32, fmt = "[%4d|%08x|%05d]", 42, 0xbeef, -42
Output: buf = "[  42|0000beef|-0042]", returns 21
```

### Example 3
```
Input:  cap = 5, fmt = "value=%d", 12345
Output: buf = "valu", returns 11      (11 >= 5, so the caller knows it was truncated)
```

### Example 4
```
Input:  buf = NULL, cap = 0, fmt = "%s=%c", "mode", 'A'
Output: nothing written, returns 6
```

### Constraints
- No `printf`-family function (`sprintf`, `snprintf`, `vsnprintf`, ...) and no conversion helpers such as `itoa` or `strtol`. From the standard library, use only `<stdarg.h>` (plus the types from `<stddef.h>` and `<stdint.h>`).
- Never write past `buf[cap - 1]`.
- Every `int` and `unsigned int` value prints correctly, `INT_MIN` and `UINT_MAX` included.
- `fmt` is a valid NUL-terminated string that uses only the conversions above, each matched by an argument of the listed type. Anything else is outside the contract.
- Python: no string formatting of any kind: no `%` on `str` or `bytes`, no f-strings, no `format()` or `str.format`, and no `str()`, `repr()`, `hex()` or `int.to_bytes` on numbers.

**Optimal complexity:** O(n) — n is the length of the complete, untruncated output
