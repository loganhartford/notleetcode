# Longest Unique Substring (ASCII)

Find the length of the longest substring that contains no repeated characters. Use a fixed-size array for tracking (no `malloc`).

```c
size_t longest_unique_substr_ascii(const char *s);
```

Use a sliding window `[lo, hi)` and a `last[256]` array that stores the last-seen index + 1 for each ASCII character (0 means unseen). When character `s[hi]` was last seen at index `last[c] - 1 >= lo`, advance `lo` past that position.

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
- Use `int last[256]` initialized to 0; index with `(unsigned char)s[hi]`
- `last[c]` stores `hi + 1` when character `c` is seen at position `hi`
- When `last[c] - 1 >= lo`, advance `lo = last[c]`

**Optimal complexity:** O(n) — each character visited at most twice
