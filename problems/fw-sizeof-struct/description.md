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

## Approach

Create an array of two `struct test_struct` objects. The byte distance from `&arr[0]` to `&arr[1]` is exactly the size of one element, including padding, because C guarantees arrays are contiguous with no gaps between elements.

```c
struct test_struct arr[2];
return (size_t)((char *)&arr[1] - (char *)&arr[0]);
```

## Why not sum the fields?

Summing field sizes misses the padding the compiler inserts for alignment. The struct above has at least one alignment gap (after `tag`, and likely after `tail`) making the true size larger than the naive sum.

## Constraints

- You may **not** use `sizeof` anywhere in your implementation.
- Do not hardcode a constant — the calculation must work in general.
- Do not sum individual field sizes.
