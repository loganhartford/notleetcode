# Store a Pointer in Raw Memory

Implement:

```c
void store_pointer(void *storage, void *value);
```

`storage` points to at least `sizeof(void *)` writable bytes.

### Behavior

```c
uint8_t slot[sizeof(void *)];
int x;

store_pointer(slot, &x);

assert(*(void **)slot == &x);
```

### Key cast

To write a pointer into a raw byte buffer, cast `storage` to `void **` — a pointer *to* a pointer — then dereference:

```c
*(void **)storage = value;
```

This is the core operation of an embedded free list: each free block's first bytes hold the address of the next free block.
