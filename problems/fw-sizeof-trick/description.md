# Implement MY_SIZEOF_TYPE(T)

Implement a macro that returns the size in bytes of a **type** `T` without using the `sizeof` operator.

```c
#define MY_SIZEOF_TYPE(T)  /* your expression */
```

The macro is called with a type name, not a variable:

```c
MY_SIZEOF_TYPE(char)      // valid
MY_SIZEOF_TYPE(int)       // valid
MY_SIZEOF_TYPE(uint64_t)  // valid
MY_SIZEOF_TYPE(int *)     // valid

int x;
MY_SIZEOF_TYPE(x)         // invalid — x is a variable, not a type
```

## Approach

Cast `0` to a `T *`, then advance it by one element with `+ 1`. Cast both addresses to `char *` and subtract — the byte distance is the size of one `T`.

```
(char *)((T *)0 + 1) - (char *)((T *)0)
```

## Constraints

- You may **not** use `sizeof` anywhere in your macro definition.
- Must work for any scalar type or pointer type passed as `T`.
