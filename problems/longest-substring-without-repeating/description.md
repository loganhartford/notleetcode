# Longest Substring Without Repeating Characters

Given a string `s`, find the length of the **longest substring** without repeating characters.

A substring is a **contiguous** sequence of characters.

### Example 1
```
Input:  s = "abcabcbb"
Output: 3        # "abc"
```

### Example 2
```
Input:  s = "bbbbb"
Output: 1        # "b"
```

### Example 3
```
Input:  s = "pwwkew"
Output: 3        # "wke" (note "pwke" is a subsequence, not a substring)
```

### Constraints
- `0 <= len(s) <= 5 * 10^4`
- `s` consists of English letters, digits, symbols and spaces.

**Optimal complexity:** O(n)  — sliding window with last-seen index
