# Implement memcpy, memmove, and memset

Implement the three fundamental memory primitives from scratch. Do not call the standard `memcpy`, `memmove`, or `memset`.

```c
void *c_memcpy(void *dst, const void *src, size_t n);
void *c_memmove(void *dst, const void *src, size_t n);
void *c_memset(void *dst, int value, size_t n);
```

- **`c_memcpy`** — copy `n` bytes from `src` to `dst`; behavior is undefined if the regions overlap (you may assume they don't)
- **`c_memmove`** — copy `n` bytes from `src` to `dst`, correctly handling overlapping regions by choosing copy direction based on pointer order
- **`c_memset`** — fill `n` bytes of `dst` with the low 8 bits of `value`
- All three return `dst`

## Examples

**Example 1** — basic copy
```
src = {1,2,3,4,5}; c_memcpy(dst, src, 5) → dst = {1,2,3,4,5}
```

**Example 2** — overlapping memmove
```
buf = {1,2,3,4,5}
c_memmove(buf+1, buf, 4)  → buf = {1,1,2,3,4}  (dst > src, copy back-to-front)
c_memmove(buf, buf+1, 4)  → buf = {1,2,3,4,4}  (dst < src, copy front-to-back)
```

**Example 3** — memset
```
c_memset(dst, 0x55, 4) → dst = {0x55, 0x55, 0x55, 0x55}
c_memset(dst, 0,    4) → dst = {0x00, 0x00, 0x00, 0x00}
```

## Constraints

- `n` may be 0 (do nothing, return `dst`)
- `c_memmove`: if `dst > src` and regions overlap, copy back-to-front; otherwise front-to-back
- `c_memset`: write `(uint8_t)(value & 0xFF)` to each byte

**Optimal complexity:** O(n)
