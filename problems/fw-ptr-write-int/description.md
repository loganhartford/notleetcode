# Write Through a Typed Pointer

Implement:

```c
void set_int(int *ptr, int value);
```

### Behavior

```c
int x = 3;
set_int(&x, 10);
assert(x == 10);
```

### Concepts
- Dereferencing a typed pointer
- Assigning through a pointer

The `*` on the left-hand side of an assignment writes *through* the pointer to the memory it addresses, rather than changing the pointer itself.
