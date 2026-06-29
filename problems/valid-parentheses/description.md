# Valid Parentheses

Given a string `s` containing just the characters `'('`, `')'`, `'{'`, `'}'`, `'['` and `']'`, determine if the input string is **valid**.

A string is valid if:
1. Open brackets are closed by the **same type** of bracket, and
2. Open brackets are closed in the **correct order**, and
3. Every close bracket has a corresponding open bracket of the same type.

### Example 1
```
Input:  s = "()[]{}"
Output: True
```

### Example 2
```
Input:  s = "(]"
Output: False
```

### Example 3
```
Input:  s = "([)]"
Output: False
```

### Constraints
- `1 <= len(s) <= 10^4`
- `s` consists only of the six bracket characters.

**Optimal complexity:** O(n)  — single-pass stack
