# Implement strcmp and strstr

Implement your own versions of two standard C string functions without calling any library string functions:

```c
int   my_strcmp(const char *a, const char *b);
char *my_strstr(const char *hay, const char *needle);
```

`my_strcmp` compares two null-terminated strings lexicographically. It returns a negative value if `a` sorts before `b`, zero if they are equal, and a positive value if `a` sorts after `b`. Only the sign of the result matters. As in the standard library, characters are compared as `unsigned char`.

`my_strstr` returns a pointer to the first occurrence of `needle` inside `hay`, or `NULL` if `needle` does not occur. An empty `needle` matches at the start, so the result is `hay`.

### Example 1
```
my_strcmp("abc", "abc")  -> 0
my_strcmp("abc", "abd")  -> negative
my_strcmp("abc", "ab")   -> positive
```

### Example 2
```
my_strstr("hello", "ll")           -> hay + 2
my_strstr("mississippi", "issip")  -> hay + 4
my_strstr("abc", "abcd")           -> NULL
my_strstr("abc", "")               -> hay
```

### Constraints
- Both arguments are valid, null-terminated strings (never `NULL`).
- Strings may contain any non-`'\0'` byte, including values `>= 0x80`.
- `my_strstr` returns a pointer into `hay`, not a copy.
- Do not use any `<string.h>` function.

**Optimal complexity:** O(n) for `my_strcmp`; O(n·m) for `my_strstr` (an O(n + m) algorithm exists but is not expected).

*Source: strstr reported in a [Qualcomm firmware interview](https://www.glassdoor.com.mx/Entrevista/Qualcomm-Entrevista-E640-RVW97821115.htm) and strcmp in [ReliSource embedded interviews](https://www.glassdoor.com.mx/Entrevista/ReliSource-Entrevista-E505414-RVW73255044.htm) (Glassdoor); strstr is LeetCode 28.*
