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
