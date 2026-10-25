# Integer Promotion Traps

The three helpers below come from a sensor driver. Each one compiles cleanly with `-Wall`, looks correct at a glance, and returns the wrong answer because of C's implicit integer conversions. Fix all three so they meet their contracts, keeping the signatures unchanged.

```c
#include <stdint.h>
#include <limits.h>

int sum_gt(unsigned a, int b, int thr) {
    return a + b > thr;
}

unsigned all_ones(void) {
    unsigned compzero = 0xFFFF;
    return compzero;
}

uint8_t inv_hi_nibble(uint8_t port) {
    return (~port) >> 4;
}
```

**`sum_gt`** returns `1` if the mathematical sum `a + b` (as if computed with unlimited range) is greater than `thr`, and `0` otherwise.

**`all_ones`** returns an `unsigned int` with every bit set, whatever the width of `unsigned int` on the target.

**`inv_hi_nibble`** takes the 8-bit bitwise inverse of `port` and returns its high nibble shifted down into bits 3..0.

### Example 1
```
Input:  sum_gt(6, -20, 6)
Output: 0            (6 + -20 = -14, which is not > 6)
```

### Example 2
```
Input:  sum_gt(UINT_MAX, 1, 0)
Output: 1            (UINT_MAX + 1 is greater than 0)
```

### Example 3
```
Input:  all_ones()
Output: 0xFFFF on a 16-bit-int target, 0xFFFFFFFF on a 32-bit-int target
```

### Example 4
```
Input:  inv_hi_nibble(0x5A)
Output: 0x0A         (~0x5A as 8 bits is 0xA5; high nibble 0xA)
```

### Constraints
- `int` may be 16 or 32 bits wide. Each fix must be correct on either.
- `sum_gt` must work for every `a` in `[0, UINT_MAX]` and every `int` value of `b` and `thr`, without undefined behavior.
- No compiler builtins.

**Optimal complexity:** O(1) per call.

Be ready to explain, for each original line, which integer promotions and usual arithmetic conversions the compiler applies and what value results.

*Source: Nigel Jones, ["A 'C' Test: The 0x10 Best Questions for Would-be Embedded Programmers"](https://web.archive.org/web/2020/https://rmbconsulting.us/publications/a-c-test-the-0x10-best-questions-for-would-be-embedded-programmers/) (Q12, Q13); the `uint8_t` case is from SEI CERT C [EXP14-C](https://wiki.sei.cmu.edu/confluence/display/c/EXP14-C.+Beware+of+integer+promotion+when+performing+bitwise+operations+on+integer+types+smaller+than+int).*
