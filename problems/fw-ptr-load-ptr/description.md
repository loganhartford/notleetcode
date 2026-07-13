# Load a Pointer from Raw Memory

Implement:

```c
void *load_pointer(const void *storage);
```

### Behavior

```c
uint8_t slot[sizeof(void *)];
int x;

*(void **)slot = &x;

assert(load_pointer(slot) == &x);
```

### Think carefully about const

`storage` is `const void *` — the data it points to should not be modified. You need to read a `void *` out of it without discarding const incorrectly.

The safest, most portable approach uses `memcpy`:

```c
void *result;
memcpy(&result, storage, sizeof(void *));
return result;
```

This avoids strict-aliasing issues and const warnings entirely. It also handles unaligned storage correctly on platforms that care.
