# C Bug Snippet Diagnosis

Each function below contains exactly one bug. Your task is to **fix** the function
so that it returns the correct result.

Bugs 1 and 2 are testable and graded by the harness. Bugs 3–8 are code-review
exercises described below — study them, understand why each is wrong, and note
the fix in a comment.

---

### Bug 1 — Operator precedence (`fix_bug1`)

```c
/* Returns 1 if n is a power of two, 0 otherwise. */
int is_pow2_buggy(unsigned n) {
    return n & (n - 1) == 0;   // BUG
}
```

### Bug 2 — Wrong toggle expression (`fix_bug2`)

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

**Bug 3 — Missing `volatile`**
```c
volatile uint8_t flag_from_isr;   // set by ISR
void wait_for_flag(void) {
    uint8_t f = flag_from_isr;
    while (f == 0) { /* spin */ }   // BUG: f cached in register
}
```
Fix: read `flag_from_isr` directly inside the loop.

**Bug 4 — `sizeof(ptr)` instead of array length**
```c
void zero_array(int *a) {
    memset(a, 0, sizeof(a));   // BUG: sizeof pointer, not array
}
```
Fix: pass the byte count explicitly, or use `sizeof(*a) * n`.

**Bug 5 — Returning a pointer to stack memory**
```c
const char *get_name(void) {
    char buf[32];
    snprintf(buf, sizeof buf, "sensor_%d", 42);
    return buf;   // BUG: buf is destroyed after return
}
```
Fix: use a static buffer, caller-provided buffer, or heap allocation.

**Bug 6 — Buffer overflow via unbounded copy**
```c
void copy_input(char *dst) {
    strcpy(dst, user_input);   // BUG: no length limit
}
```
Fix: use `strncpy` or `strlcpy` with an explicit size.

**Bug 7 — Signed left shift / overflow UB**
```c
int32_t set_high_bit(void) {
    return 1 << 31;   // BUG: UB — signed overflow
}
```
Fix: `(int32_t)(1u << 31)` or `INT32_MIN`.

**Bug 8 — Missing bounds check before register access**
```c
#define NUM_REGS 8
uint32_t regs[NUM_REGS];
void write_reg(int idx, uint32_t val) {
    regs[idx] = val;   // BUG: idx unchecked
}
```
Fix: guard with `if (idx < 0 || idx >= NUM_REGS) return;`

---

### Constraints
- Fix only the marked bug; leave everything else unchanged.
- Do not use library functions to bypass the intent of Bug 1/2.

**Optimal complexity:** O(1) — all fixes are single-expression corrections
