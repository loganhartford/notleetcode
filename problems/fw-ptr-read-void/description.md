# Read Through a void *

Implement:

```c
int read_int(const void *ptr);
```

### Behavior

```c
int x = 42;
assert(read_int(&x) == 42);
```

### Constraint
Cast `ptr` to the correct pointer type before dereferencing.

You cannot dereference a `void *` directly — the compiler does not know how many bytes to read or how to interpret them. You must cast first:

```c
*(const int *)ptr
```
