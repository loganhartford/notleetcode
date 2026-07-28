# Endianness Flip Macro

When a 32-bit value crosses between a big-endian bus (network packets, many SPI sensors) and a
little-endian MCU, its four bytes must be reversed. Write a **preprocessor macro**
`FLIP_ENDIANNESS(x)` that returns `x` with its byte order swapped:

```
byte 0 (LSB) <-> byte 3 (MSB)
byte 1       <-> byte 2
```

```c
#define FLIP_ENDIANNESS(x)  /* your macro here */
```

### Why a macro (and the catch)

The interviewer specifically asks for a **macro**, not a function — and the point is macro
hygiene. A macro is raw text substitution, so a naive version breaks in two classic ways:

- **No outer parentheses.** The body is a chain of `|`s. If the whole expression isn't wrapped
  in `( ... )`, then `FLIP_ENDIANNESS(v) >> 8` or `mask & FLIP_ENDIANNESS(v)` binds the outer
  operator into the middle of the chain and returns garbage.
- **No parentheses around `x`.** Each use of the argument must be `((uint32_t)(x))`. Otherwise
  `FLIP_ENDIANNESS(a | b)` expands to `a | b & 0x000000FF ...`, and `&`'s higher precedence
  silently corrupts the result.

Your macro must produce the correct value in **all** of those contexts, so parenthesize the
whole body and every use of `x`, and cast `x` to `uint32_t` so shifts are unsigned.

### Examples

```
FLIP_ENDIANNESS(0x12345678) -> 0x78563412
FLIP_ENDIANNESS(0xAABBCCDD) -> 0xDDCCBBAA
FLIP_ENDIANNESS(0x00000001) -> 0x01000000
FLIP_ENDIANNESS(0x000000FF) -> 0xFF000000
FLIP_ENDIANNESS(0x00000000) -> 0x00000000
```

Applying it twice is the identity: `FLIP_ENDIANNESS(FLIP_ENDIANNESS(v)) == v`.

### Constraints

- Implement it as a `#define`, using only shifts and masks — no `__builtin_bswap32`, no
  unions, no `memcpy`, no helper function.
- The argument is a 32-bit unsigned quantity (`uint32_t` from `<stdint.h>`).

**Optimal complexity:** O(1) — a fixed set of shifts and masks.
