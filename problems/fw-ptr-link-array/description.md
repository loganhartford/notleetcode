# Build a Chain of Blocks

Implement:

```c
void link_block_array(void *storage, size_t block_count, size_t block_size);
```

### Behavior

For:

```c
uint8_t memory[64];
link_block_array(memory, 4, 16);
```

The result should be:

```
block 0 → block 1 → block 2 → block 3 → NULL
```

Verified by:

```c
void *b0 = &memory[0];
void *b1 = &memory[16];
void *b2 = &memory[32];
void *b3 = &memory[48];

assert(*(void **)b0 == b1);
assert(*(void **)b1 == b2);
assert(*(void **)b2 == b3);
assert(*(void **)b3 == NULL);
```

This is the initialization step of a static memory pool: carve a flat buffer into equally-sized blocks and link them into a free list.
