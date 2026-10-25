# Preprocessor Macro Pitfalls

Write three macros in your solution file. The harness `#include`s it and uses each macro the way real driver code would: as a constant, inside larger expressions, with expressions as arguments, and as the body of an `if`. A macro is pasted in as text, so each one has to give the right answer in every one of those contexts.

```c
#define SECONDS_PER_YEAR          /* seconds in a 365-day year */
#define MIN(A, B)                 /* the smaller of A and B */
#define WRITE_REG16(hi, lo, val)  /* write a 16-bit value to two 8-bit registers */
```

**`SECONDS_PER_YEAR`** is the number of seconds in a year, ignoring leap years. It must have type `unsigned long`, and the definition must also be correct when compiled for a target where `int` is 16 bits.

**`MIN(A, B)`** is the "standard" MIN macro. It evaluates to the smaller of its two arguments and must stay correct when an argument is an expression or when the result is an operand of a larger expression.

**`WRITE_REG16(hi, lo, val)`** writes the 16-bit value `val` to a pair of 8-bit registers: the high byte to `hi`, then the low byte to `lo`. Order matters: on parts such as AVR's 16-bit timers, the low-byte write latches both bytes, so the high byte must go first. `val` may have side effects (for example, popping a FIFO) and must be evaluated exactly once. The macro must work anywhere a single statement can appear, including here:

```c
if (use_a)
    WRITE_REG16(TA_HI, TA_LO, reload);
else
    WRITE_REG16(TB_HI, TB_LO, reload);
```

Bare braces `{ ... }` are not enough for that. Be ready to explain why, and what idiom fixes it.

### Example 1
```
Input:  SECONDS_PER_YEAR
Output: 31536000UL
```

### Example 2
```
Input:  x = 4, y = 2;  10 / MIN(x, y)
Output: 5
```

### Example 3
```
Input:  x = 0x13;  MIN(x & 0xF, 7)
Output: 3
```

### Example 4
```
Input:  WRITE_REG16(hi, lo, 0xBEEF)
Output: hi = 0xBE, lo = 0xEF   (hi written first)
```

### Constraints
- All three must be `#define`s, not functions.
- `MIN` must be a plain C expression: no GNU statement expressions or `typeof`. It may evaluate an argument more than once. Be ready to say what `MIN(*p++, b)` does.
- `WRITE_REG16` evaluates each of `hi`, `lo` and `val` exactly once. `hi` and `lo` are modifiable `uint8_t` lvalues (real registers would be `volatile uint8_t`).
- Do not rely on anything beyond C11 and `<stdint.h>`.

Be ready to explain what goes wrong on a 16-bit-`int` target if `SECONDS_PER_YEAR` is written without its suffixes.

*Source: Nigel Jones, ["A 'C' Test: The 0x10 Best Questions for Would-be Embedded Programmers"](https://web.archive.org/web/2020/https://rmbconsulting.us/publications/a-c-test-the-0x10-best-questions-for-would-be-embedded-programmers/) (Q1, Q2); the multi-statement macro idiom is [comp.lang.c FAQ 10.4](https://c-faq.com/cpp/multistmt.html).*
