# Command Line Tokenizer

Implement a tokenizer that splits a command line string in-place. This is the first step in building a simple embedded command interpreter: split the input on whitespace, null-terminate each token, and populate a `argv`-style array of pointers.

```c
size_t parse_tokens(char *line, char **argv, size_t max_args);
```

- Modifies `line` in place by replacing whitespace separators with `'\0'`
- Fills `argv[0..n-1]` with pointers into `line`
- Returns `n`, the number of tokens found (capped at `max_args`)
- Skip leading, trailing, and repeated whitespace (space `' '` and tab `'\t'`)

## Examples

**Example 1** — basic tokenization
```
line = "cmd arg1 arg2"
parse_tokens(line, argv, 8) → 3
argv[0]="cmd", argv[1]="arg1", argv[2]="arg2"
```

**Example 2** — whitespace handling
```
line = "  cmd   arg  "
parse_tokens(line, argv, 8) → 2
argv[0]="cmd", argv[1]="arg"
```

**Example 3** — edge cases
```
parse_tokens("", argv, 8)      → 0
parse_tokens("cmd", argv, 8)   → 1
parse_tokens("a b c", argv, 2) → 2  (capped at max_args)
```

## Constraints

- Whitespace = `' '` (space) and `'\t'` (tab) only
- `line` is a writable, null-terminated C string
- Returns at most `max_args` tokens; extra tokens are silently truncated
- `argv` must not be written past index `max_args - 1`

**Optimal complexity:** O(n) — single pass through the string
