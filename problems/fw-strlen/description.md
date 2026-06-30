# Implement strlen

Implement your own version of the standard `strlen` function. Walk the string pointer until the null terminator is found and return the number of characters.

```c
size_t c_strlen(const char *s);
```

Do not call the standard `strlen`. Use only pointer arithmetic.

## Examples

**Example 1**
```
c_strlen("")          → 0
c_strlen("a")         → 1
c_strlen("hello")     → 5
```

**Example 2**
```
c_strlen("notleetcode") → 11
c_strlen("12345")       → 5
```

**Example 3**
```
c_strlen("abcde")  → 5
c_strlen("x")      → 1
```

## Constraints

- `s` is a valid, null-terminated C string
- Return type is `size_t`
- Do not use `strlen` or any other string library function

**Optimal complexity:** O(n) — single pointer walk
