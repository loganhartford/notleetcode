# Basic Calculator II

Evaluate a mathematical expression string containing non-negative integers, `+`, `-`, `*`, `/`, and spaces. Integer division truncates toward zero.

```cpp
int calculate(string s);
```

No parentheses are present. Operators follow standard precedence: `*` and `/` before `+` and `-`.

## Examples

```
calculate("3 + 2 * 2")   → 7
calculate(" 3/2 ")        → 1
calculate(" 3+5 / 2 ")   → 5
calculate("2 + 3 * 4")   → 14
calculate("3 - 4 * 2")   → -5
```

## Constraints

- `1 <= s.length <= 3 * 10^5`
- `s` contains digits, `+`, `-`, `*`, `/`, and spaces only
- All intermediate and final values fit in a 32-bit integer

**Optimal complexity:** O(n) — single pass with a stack; push terms for `+`/`-`, apply `*`/`/` immediately.
