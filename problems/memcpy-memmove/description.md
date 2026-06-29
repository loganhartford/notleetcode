# memcpy and memmove

Implement both `memcpy` and `memmove` from scratch, and understand why they differ.

```c
void* my_memcpy(void* dst, const void* src, size_t n);
void* my_memmove(void* dst, const void* src, size_t n);
```

- Both copy `n` bytes from `src` to `dst` and **return `dst`**.
- `my_memcpy` may assume the regions **do not overlap** (copy front-to-back).
- `my_memmove` must work **even when the regions overlap** — e.g. shifting a
  buffer in place. The trick: if `dst > src`, copy **back-to-front** so you don't
  clobber bytes you haven't read yet; otherwise copy front-to-back.

### Example
```
char b[] = "hello world";
my_memcpy(b, b + 6, 5);   // non-overlapping enough: b -> "world world"

char c[] = "abcdef";
my_memmove(c + 2, c, 4);  // overlapping forward shift -> "ababcd"
```

### Constraints
- Operate byte-wise (`unsigned char*`); don't call the real `memcpy`/`memmove`.
- `[author]`-defined firmware practice.

**Optimal complexity:** O(n)  — one copy per byte
