# Reverse a C String In Place

Implement a function that reverses a null-terminated C string in place, modifying the buffer directly without allocating any memory.

```c
void reverse_cstr(char *s);
```

The reversal must handle edge cases gracefully:
- Empty string (`""`) — do nothing
- Single character — do nothing (already reversed)
- Palindromes — the result equals the input

## Examples

**Example 1**
```
s = "hello"  →  s becomes "olleh"
```

**Example 2**
```
s = "ab"     →  s becomes "ba"
s = "racecar" → s stays "racecar"
```

**Example 3**
```
s = ""       →  s stays ""      (empty — nothing to do)
s = "x"      →  s stays "x"    (single char — nothing to do)
```

## Constraints

- `s` is a valid, writable, null-terminated C string
- No allocation or libc string functions allowed — use only pointer arithmetic

**Optimal complexity:** O(n) — single pass with two pointers
