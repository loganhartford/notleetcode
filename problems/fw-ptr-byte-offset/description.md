# Generic Byte Offset

Implement:

```c
void *byte_offset(void *base, size_t offset);
```

### Behavior

```c
uint8_t data[16];
assert(byte_offset(data, 5) == &data[5]);
```

### Constraint
- Do **not** use `uintptr_t`.
- Perform byte-based pointer arithmetic only.

You cannot do arithmetic on a `void *` directly in C. Cast to `uint8_t *` first — a `uint8_t` is exactly one byte, so adding `offset` advances by exactly `offset` bytes.
