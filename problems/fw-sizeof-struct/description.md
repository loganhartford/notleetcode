# Find Struct Size Without sizeof

The following struct is defined for you:

```c
struct test_struct {
    char     tag;
    uint32_t id;
    uint16_t flags;
    double   value;
    char     tail;
};
```

Implement a function that returns its size in bytes **without using `sizeof`**:

```c
size_t struct_size_without_sizeof(void);
```

The returned value must include all internal and trailing padding — it must match what `sizeof(struct test_struct)` would return.

## Constraints

- You may **not** use `sizeof` anywhere in your implementation.
- Do not hardcode a constant.
