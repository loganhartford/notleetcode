# Implement sizeof Without sizeof

Implement a macro `MY_SIZEOF(T)` that returns the size in bytes of any type `T` — **without using the `sizeof` operator** anywhere in your implementation.

## Signature

```c
#define MY_SIZEOF(T)  /* your expression here */
```

## Approach

Pointer arithmetic: if you subtract two pointers of type `T*`, the result is the number of `T`-sized elements between them. Use a null pointer cast to force the compiler to compute the byte distance between element 0 and element 1 without allocating anything.

## Examples

```
MY_SIZEOF(char)     → 1
MY_SIZEOF(int)      → 4
MY_SIZEOF(double)   → 8
MY_SIZEOF(uint64_t) → 8
```

## Constraints

- You may not use `sizeof` anywhere in your macro definition.
- The macro must work for any scalar type passed as `T`.
- Must evaluate to a compile-time-usable constant expression (no runtime overhead).
