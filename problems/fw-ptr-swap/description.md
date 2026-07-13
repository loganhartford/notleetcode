# Generic Byte Swap

Implement:

```c
void swap_values(void *a, void *b, size_t size);
```

### Behavior

```c
int x = 10, y = 20;
swap_values(&x, &y, sizeof(int));
assert(x == 20);
assert(y == 10);
```

### Constraints
- Treat memory as raw bytes — do not assume the underlying type.
- `size` can be any positive number of bytes.

### Approach

Cast `a` and `b` to `uint8_t *` and swap byte-by-byte using a single-byte temporary:

```c
uint8_t tmp = pa[i]; pa[i] = pb[i]; pb[i] = tmp;
```

This works for any type: `int`, `double`, structs, pointers. It is also how `qsort` in libc swaps elements when it does not know the element type at compile time.
