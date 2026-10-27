# C Bug Snippet Diagnosis

Each function below contains exactly one bug. Your task is to **fix** the function
so that it returns the correct result.

Bugs 1 and 2 are testable and graded by the harness. Bugs 3–8 are code-review
exercises described below — study them, understand why each is wrong, and note
the fix in a comment.

---

### Bug 1 (`fix_bug1`)

```c
/* Returns 1 if n is a power of two, 0 otherwise. */
int is_pow2_buggy(unsigned n) {
    return n & (n - 1) == 0;   // BUG
}
```

### Bug 2 (`fix_bug2`)

```c
/* Clears bit i in reg, toggles bit j. Returns the result. */
uint32_t toggle_and_clear(uint32_t reg, unsigned i, unsigned j) {
    reg &= ~(1u << i);
    reg ^= ~(1u << j);   // BUG
    return reg;
}
```

---

### Code-review bugs (no harness tests — study and fix in comments)

**Bug 3**
```c
volatile uint8_t flag_from_isr;   // set by ISR
void wait_for_flag(void) {
    uint8_t f = flag_from_isr;
    while (f == 0) { /* spin */ }   // BUG
}
```

**Bug 4**
```c
void zero_array(int *a) {
    memset(a, 0, sizeof(a));   // BUG
}
```

**Bug 5**
```c
const char *get_name(void) {
    char buf[32];
    snprintf(buf, sizeof buf, "sensor_%d", 42);
    return buf;   // BUG
}
```

**Bug 6**
```c
void copy_input(char *dst) {
    strcpy(dst, user_input);   // BUG
}
```

**Bug 7**
```c
int32_t set_high_bit(void) {
    return 1 << 31;   // BUG
}
```

**Bug 8**
```c
#define NUM_REGS 8
uint32_t regs[NUM_REGS];
void write_reg(int idx, uint32_t val) {
    regs[idx] = val;   // BUG
}
```

<details><summary>Answers for bugs 3–8</summary>

- Bug 3: the volatile flag is copied into `f` once, so the loop never sees the ISR's write. Fix: read `flag_from_isr` directly inside the loop.

- Bug 4: `sizeof(a)` is the size of the pointer, not of the array. Fix: pass the byte count explicitly, or use `sizeof(*a) * n`.

- Bug 5: returns a pointer to a stack buffer that is gone after the return. Fix: use a static buffer, caller-provided buffer, or heap allocation.

- Bug 6: unbounded copy overflows `dst`. Fix: pass the destination capacity and leave room for the terminator. Note that `strncpy` does not terminate the destination when the source is too long, so it is not a drop-in fix on its own.

- Bug 7: `1 << 31` overflows a signed `int` (undefined behavior). Fix: use `INT32_MIN` for the signed value, or `UINT32_C(1) << 31` for an unsigned mask. Converting an out-of-range unsigned value to a signed type is implementation-defined in C11.

- Bug 8: `idx` is never range-checked. Fix: guard with `if (idx < 0 || idx >= NUM_REGS) return;`

Bug 1 is operator precedence (`==` binds tighter than `&`); bug 2 XORs with an inverted mask.

</details>

---

### Constraints
- Fix only the marked bug; leave everything else unchanged.
- Do not use library functions to bypass the intent of Bug 1/2.

**Optimal complexity:** O(1)
