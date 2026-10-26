# Hex Encode and Decode

Implement both directions of a hex codec: parse a hex string into raw bytes, and format raw bytes as a hex string. Do it without calling any library conversion functions.

```c
int hex_to_bytes(const char *hex, uint8_t *out, size_t out_cap);
int bytes_to_hex(const uint8_t *in, size_t n, char *out, size_t out_cap);
```

**`hex_to_bytes`** reads the NUL-terminated string `hex`. Each byte is two hex digits, most significant nibble first. The letters `a`–`f` may be upper or lower case. There is no `0x` prefix and there are no spaces or separators. Write the bytes to `out` and return how many were written. The empty string returns 0. Return `-1` if the string has odd length, contains any character that is not a hex digit, or would need more than `out_cap` bytes. On error `out` may hold partial output, but nothing may ever be written past `out[out_cap - 1]`.

**`bytes_to_hex`** writes the `n` bytes of `in` to `out` as `2 * n` lowercase hex digits followed by `'\0'`, and returns the number of characters written, not counting the `'\0'`. If `out_cap < 2 * n + 1`, return `-1` and write nothing to `out` at all. With `n == 0` and `out_cap >= 1`, write `""` and return 0.

Python: `hex_to_bytes(hexstr: bytes, out: bytearray) -> int` and `bytes_to_hex(data: bytes, out: bytearray) -> int`. `hexstr` ends in `b"\0"` like a C string, `out_cap` is `len(out)`, and `bytes_to_hex` writes ASCII digit bytes and then a `0` byte into `out`.

### Example 1
```
Input:  hex_to_bytes("deadBEEF", out, out_cap = 4)
Output: returns 4, out = {0xDE, 0xAD, 0xBE, 0xEF}
```

### Example 2
```
Input:  hex_to_bytes("abc", out, out_cap = 4)
Output: returns -1      (odd length)
```

### Example 3
```
Input:  hex_to_bytes("0123456789", out, out_cap = 4)
Output: returns -1      (needs 5 bytes, only 4 available)
```

### Example 4
```
Input:  bytes_to_hex({0x00, 0xFF, 0x7F, 0x80}, n = 4, out, out_cap = 9)
Output: returns 8, out = "00ff7f80"
```

### Example 5
```
Input:  bytes_to_hex({0x00, 0xFF, 0x7F, 0x80}, n = 4, out, out_cap = 8)
Output: returns -1, out untouched
```

### Constraints
- No `sscanf`, `strtol`, `snprintf`, or any other library conversion function. Python: no `bytes.fromhex`, `.hex()`, `int(s, 16)`, `binascii`, or format strings
- `hex` is a valid NUL-terminated string; `in` holds `n` bytes; `out` has at least `out_cap` bytes; the buffers do not overlap
- `out_cap` may be 0
- Never write past `out[out_cap - 1]`
- All lengths fit in an `int`

**Optimal complexity:** O(n) time, O(1) extra space
