# Push onto an Embedded Free List

Implement:

```c
void free_list_push(void **head, void *block);
```

Given `head → B → C → NULL` and `block == A`, produce:

```
head → A → B → C → NULL
```

### Behavior

```c
uint8_t a[16], b[16], c[16];
*(void **)b = c;
*(void **)c = NULL;
void *head = b;

free_list_push(&head, a);

assert(head == a);
assert(*(void **)a == b);
```

### Note
This is the core of `pool_free`. Once you have `free_list_pop` and `free_list_push` working, a full pool allocator is just bookkeeping (bounds checking, alignment, capacity counting) around these two operations.
