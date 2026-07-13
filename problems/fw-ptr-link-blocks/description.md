# Link Two Memory Blocks

Given two raw memory blocks, implement:

```c
void link_blocks(void *current, void *next);
```

### Behavior

```c
uint8_t block_a[16];
uint8_t block_b[16];

link_blocks(block_a, block_b);
assert(*(void **)block_a == block_b);

link_blocks(block_b, NULL);
assert(*(void **)block_b == NULL);
```

### Concept

An embedded free list stores the pointer to the next block in the first `sizeof(void *)` bytes of each free block. No separate node struct is needed — the block itself is the node.

`link_blocks(a, b)` makes block `a` point to block `b` by writing `b`'s address into `a`'s first word.
