# Detect Stack Growth Direction

Implement a function that determines whether the call stack grows **upward**
(toward higher addresses) or **downward** (toward lower addresses) on the
current platform.

Use the addresses of local variables in nested function calls to infer the
direction — no platform-specific macros or assembly allowed.

```c
typedef enum {
    STACK_UNKNOWN  = 0,
    STACK_GROWS_UP,
    STACK_GROWS_DOWN
} stack_dir_t;

stack_dir_t detect_stack_direction(void);
```

### How it works

Take the address of a local variable in the outer call frame, then call a
helper that takes the address of its own local variable. Compare the two
addresses:

- If the inner address is **higher**, the stack grows **up**.
- If the inner address is **lower**, the stack grows **down**.

### Example

```
outer frame:  &outer_local  =  0x7ffee4b00
inner frame:  &inner_local  =  0x7ffee4a10   ← lower → grows DOWN
```

### Note on portability

Stack direction is platform-defined behaviour. On all mainstream architectures
(x86-64, ARM Cortex-M, RISC-V) the stack grows **downward**. The harness
accepts either direction but expects a definite answer (not `STACK_UNKNOWN`).

### Constraints
- No inline assembly.
- No `__builtin_frame_address` or platform headers.
- The helper that compares addresses must be a separate non-inlined function.

**Optimal complexity:** O(1) — two address comparisons
