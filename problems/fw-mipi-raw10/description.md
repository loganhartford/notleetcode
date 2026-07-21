# MIPI RAW10 → PLAIN16 Unpacking

MIPI RAW10 is a packed image format from the MIPI CSI-2 spec. It stores 10-bit sensor pixels with minimal bandwidth: every group of 4 pixels (4 × 10 = 40 bits) is packed into exactly 5 consecutive bytes.

### Byte layout for pixels P0, P1, P2, P3

```
Byte 0 : P0[9:2]  — 8 MSBs of pixel 0
Byte 1 : P1[9:2]  — 8 MSBs of pixel 1
Byte 2 : P2[9:2]  — 8 MSBs of pixel 2
Byte 3 : P3[9:2]  — 8 MSBs of pixel 3
Byte 4 : bits [1:0] → 2 LSBs of P0
         bits [3:2] → 2 LSBs of P1
         bits [5:4] → 2 LSBs of P2
         bits [7:6] → 2 LSBs of P3
```

PLAIN16 stores each 10-bit value zero-padded into the lower 10 bits of a `uint16_t` (bits `[15:10]` are always 0).

Implement:

```c
void mipiRaw10ToPlain16(const uint8_t *raw10,
                        uint16_t      *plain16,
                        int            pixelCount);
```

- `raw10` — packed input buffer; size in bytes is `(pixelCount / 4) * 5`
- `plain16` — output buffer of `pixelCount` elements; caller-allocated
- `pixelCount` — always ≥ 4 and a multiple of 4

### Example 1 — four pixels

Pixels: `1023 (0x3FF)`, `0`, `341 (0x155)`, `682 (0x2AA)`

```
Byte 0 = 0xFF   (1023 >> 2)
Byte 1 = 0x00   (0 >> 2)
Byte 2 = 0x55   (341 >> 2)
Byte 3 = 0xAA   (682 >> 2)
Byte 4 = 0x93   (2<<6 | 1<<4 | 0<<2 | 3)

raw10[]   = { 0xFF, 0x00, 0x55, 0xAA, 0x93 }
plain16[] = { 1023, 0, 341, 682 }
```

### Example 2 — eight pixels all equal to 512

```
raw10[]   = { 0x80,0x80,0x80,0x80,0x00, 0x80,0x80,0x80,0x80,0x00 }
plain16[] = { 512,512,512,512, 512,512,512,512 }
```

### Constraints

- `4 ≤ pixelCount ≤ 10,000,000`, always a multiple of 4
- Each reconstructed pixel value is in `[0, 1023]`
- Both pointers are valid and non-overlapping

**Optimal complexity:** O(n) — one pass, 5 bytes in → 4 pixels out per iteration
