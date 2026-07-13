# Write Through a void *

Implement:

```c
void write_int(void *ptr, int value);
```

### Behavior

```c
int x = 0;
write_int(&x, 99);
assert(x == 99);
```

### Key distinction

```c
ptr = &something;      // moves the pointer — does NOT write to the address
*(int *)ptr = value;   // writes value to the address ptr holds
```

The `(int *)` cast tells the compiler "treat this address as a pointer to int", then `*` dereferences it to perform the write.
