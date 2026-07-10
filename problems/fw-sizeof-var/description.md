# Implement MY_SIZEOF_VAR(x)

Implement a macro that returns the size in bytes of a **variable or object** `x` without using the `sizeof` operator.

```c
#define MY_SIZEOF_VAR(x)  /* your expression */
```

The macro is called with a variable or expression, not a type:

```c
int x;
double d;
uint8_t buf[16];

MY_SIZEOF_VAR(x)    // valid — variable
MY_SIZEOF_VAR(d)    // valid — variable
MY_SIZEOF_VAR(buf)  // valid — array (returns full array size, not pointer size)

MY_SIZEOF_VAR(int)  // invalid — int is a type, not a variable
```

## Key distinction from MY_SIZEOF_TYPE

For arrays, `MY_SIZEOF_VAR(arr)` returns the **full array size** because `&arr` has type pointer-to-array, not pointer-to-element. This is one case where the variable form gives you different (and more useful) information than the type form.

## Approach

Take the address of `x` with `&(x)` — this gives a pointer to the object. Advance it by one with `+ 1` to point one object past `x`. Cast both to `char *` and subtract.

```
(char *)(&(x) + 1) - (char *)&(x)
```

## Constraints

- You may **not** use `sizeof` anywhere in your macro definition.
- Must work for scalars, pointers, and arrays.
