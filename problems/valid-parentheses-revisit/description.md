# Valid Parentheses (revisit)

A revisit — this time with the **monotonic / stack mindset** front and center.

Given a string `s` containing just `'('`, `')'`, `'{'`, `'}'`, `'['` and `']'`, determine if the input string is **valid**: open brackets must be closed by the same type, in the correct order, and every close must have a matching open.

### Example 1
```
Input:  s = "()[]{}"
Output: True
```

### Example 2
```
Input:  s = "([)]"
Output: False
```

### Constraints
- `1 <= len(s) <= 10^4`
- `s` consists only of the six bracket characters.
