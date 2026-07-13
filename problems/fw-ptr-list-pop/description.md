# Pop from an Embedded Free List

Implement:

```c
void *free_list_pop(void **head);
```

Given a list `A → B → C → NULL`, it should:
- Return `A`
- Advance `*head` to `B`

### Behavior

```c
uint8_t a[16], b[16], c[16];
*(void **)a = b;
*(void **)b = c;
*(void **)c = NULL;
void *head = a;

void *result = free_list_pop(&head);

assert(result == a);
assert(head == b);
```

If `*head` is `NULL` (empty list), return `NULL` without crashing.

### Note
This is `pool_alloc` stripped of all bookkeeping — just the pointer manipulation at its core.
