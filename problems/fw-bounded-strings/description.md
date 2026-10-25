# Implement strlcpy

Implement a size-bounded C string copy without calling library functions.

```c
size_t c_strlcpy(char *dst, const char *src, size_t dstsize);
```

- Copy at most `dstsize - 1` characters from `src`. If `dstsize > 0`, write a terminating `\0` after the copied characters.
- Always return the full length of `src`, excluding its terminator, even when the copy is truncated.
- If `dstsize == 0`, write nothing and still return the source length.

`src` is a valid NUL-terminated string. `dst` is a valid buffer with at least `dstsize` bytes. The buffers do not overlap. The destination need not already contain a string. Bytes after the result's terminator within the capacity are unspecified.

### Examples

Each row starts with a separate writable destination buffer.

| Call | Result in destination | Return |
| --- | --- | --- |
| `c_strlcpy(dst, "cat", 4)` | `"cat\0"` | `3` |
| `c_strlcpy(dst, "hello", 4)` | `"hel\0"` | `5` |
| `c_strlcpy(dst, "hello", 0)` | No writes | `5` |

Python uses `c_strlcpy(dst: bytearray, src: bytes, dstsize: int) -> int`, with NUL-terminated source bytes and the same contract.

### Source

[42 Piscine C02, exercise 10](https://raw.githubusercontent.com/kuvarti/42k-Piscine/main/PDFs/EN_C02.pdf) asks for a `strlcpy` implementation. This version uses a `c_` prefix, `size_t`, and `const` source parameter; behavior follows the [OpenBSD strlcpy manual](https://man.openbsd.org/strlcpy).

**Complexity:** O(source length) time, O(1) extra space.
