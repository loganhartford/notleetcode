# Is the Stack Growing Up?

On most architectures the call stack grows **downward** — each nested function call places its
frame at a *lower* address than its caller. But that's a platform decision, not a language one.
Detect it at runtime:

```c
bool is_stack_growing_up(void);
```

Return `true` if the stack grows toward **higher** addresses, `false` if it grows toward
**lower** addresses. No inline assembly, no platform macros, no `__builtin_frame_address`.

### The reliable technique: compare across frames

Take the address of a local variable in one frame, then call a helper that takes the address of
*its own* local. Because the helper's frame sits on top of the caller's, comparing the two
addresses reveals the growth direction:

```
outer frame:  &outer_local
    |
    v  call helper
inner frame:  &inner_local
```

- inner address **higher** than outer → stack grows **up** → return `true`
- inner address **lower** than outer → stack grows **down** → return `false`

### Why not just compare two locals in the same function?

The tempting shortcut — declare `a`, then `b`, and compare `&a` vs `&b` — does **not** work
reliably. The compiler is free to lay out locals within a single frame in any order (and often
reorders or coalesces them entirely), so their relative addresses tell you nothing about which
way the stack grows. You need two *different* frames, and the helper must not be inlined —
otherwise it shares the caller's frame and there's nothing to compare. Mark it
`__attribute__((noinline))`.

### Example

```
&outer_local = 0x7ffee4b00
&inner_local = 0x7ffee4a10   (lower)  ->  grows down  ->  returns false
```

### Constraints

- No inline assembly, no `__builtin_frame_address`, no platform-specific headers.
- The address-comparing helper must be a separate, non-inlined function.
- Must return a definite `true`/`false` — the two addresses are never equal across real frames.

**Optimal complexity:** O(1) — two address comparisons.
