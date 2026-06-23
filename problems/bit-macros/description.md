# Set / Clear / Toggle / Test a Bit (the macros)

Write the four classic firmware bit-manipulation **macros**. These are the
register-poking primitives you write constantly in embedded code.

```c
#define SET_BIT(reg, n)     // set bit n of reg (in place)
#define CLEAR_BIT(reg, n)   // clear bit n of reg (in place)
#define TOGGLE_BIT(reg, n)  // flip bit n of reg (in place)
#define TEST_BIT(reg, n)    // evaluate to 1 if bit n is set, else 0
```

- `SET_BIT`, `CLEAR_BIT`, and `TOGGLE_BIT` modify `reg` **in place** (they are used
  as statements: `SET_BIT(reg, 3);`).
- `TEST_BIT` is an **expression** that evaluates to `1` or `0`.
- `n` is a bit index from `0` (LSB) to `31`.

### Example
```c
uint32_t r = 0;
SET_BIT(r, 3);      // r == 0b1000  == 8
SET_BIT(r, 0);      // r == 0b1001  == 9
TEST_BIT(r, 3);     // 1
TEST_BIT(r, 1);     // 0
CLEAR_BIT(r, 3);    // r == 0b0001  == 1
TOGGLE_BIT(r, 5);   // r == 0b100001 == 33
TOGGLE_BIT(r, 5);   // r == 1
```

### Constraints
- Operate on `uint32_t` registers.
- This is `[author]`-defined firmware practice — no LeetCode equivalent.
