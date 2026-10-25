# Implement strlcpy

Implement `strlcpy`, the size-bounded string copy from BSD, without calling any library functions.

```c
size_t c_strlcpy(char *dst, const char *src, size_t dstsize);
```

`dstsize` is the total size of the `dst` buffer in bytes, including room for the terminator. Copy as much of `src` as fits (at most `dstsize - 1` characters), then write `'\0'` after the copied characters. If `dstsize` is 0, write nothing.

Return the length of `src` — its full length, even when the copy was truncated. That lets the caller detect truncation with `if (ret >= dstsize)`.

### Example 1
```
Input:  src = "cat", dstsize = 4
Output: dst = "cat", returns 3
```

### Example 2
```
Input:  src = "hello", dstsize = 4
Output: dst = "hel", returns 5      (5 >= 4, so the caller knows it was truncated)
```

### Example 3
```
Input:  src = "hello", dstsize = 0
Output: dst untouched, returns 5
```

### Constraints
- `src` is a valid NUL-terminated string; `dst` has at least `dstsize` bytes; the buffers do not overlap
- Never write past `dst[dstsize - 1]`
- Python: `dst` is a `bytearray`, `src` is NUL-terminated `bytes`

**Optimal complexity:** O(n) time, O(1) space — one pass over `src`
