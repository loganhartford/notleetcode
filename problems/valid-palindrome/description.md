# Valid Palindrome

A phrase is a **palindrome** if, after converting all uppercase letters to lowercase and removing all non-alphanumeric characters, it reads the same forward and backward.

Given a string `s`, return `True` if it is a palindrome, or `False` otherwise.

### Example 1
```
Input:  s = "A man, a plan, a canal: Panama"
Output: True        # "amanaplanacanalpanama"
```

### Example 2
```
Input:  s = "race a car"
Output: False       # "raceacar"
```

### Example 3
```
Input:  s = " "
Output: True        # empty string after filtering reads the same both ways
```

### Constraints
- `1 <= len(s) <= 2 * 10^5`
- `s` consists of printable ASCII characters.

**Optimal complexity:** O(n)  — two-pointer single pass
