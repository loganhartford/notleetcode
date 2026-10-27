# Longest Unique Substring (ASCII)

Find the length of the longest substring that contains no repeated characters, without `malloc`.

```c
size_t longest_unique_substr_ascii(const char *s);
```

## Examples

**Example 1**
```
longest_unique_substr_ascii("")       → 0
longest_unique_substr_ascii("abcabc") → 3  ("abc")
longest_unique_substr_ascii("aaaaaa") → 1  ("a")
```

**Example 2**
```
longest_unique_substr_ascii("abcd")   → 4  (whole string)
longest_unique_substr_ascii("pwwkew") → 3  ("wke")
```

**Example 3**
```
longest_unique_substr_ascii("dvdf")   → 3  ("vdf")
longest_unique_substr_ascii("a")      → 1
```

## Constraints

- ASCII characters only (values 0–255)
- No `malloc`; O(1) extra space

**Optimal complexity:** O(n)
