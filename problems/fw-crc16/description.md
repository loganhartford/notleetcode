# CRC-16/CCITT-FALSE

Implement CRC-16/CCITT-FALSE, the variant used in Xmodem, HDLC, and many embedded protocols.

```c
uint16_t crc16_ccitt(const uint8_t *data, size_t len);
```

Parameters:
- **Polynomial:** 0x1021
- **Initial value:** 0xFFFF
- **Input reflection:** none (MSB first)
- **Output reflection:** none
- **Final XOR:** none

The canonical check value: feeding `"123456789"` (ASCII, 9 bytes) must return `0x29B1`.

## Examples

**Example 1** — check value
```
crc16_ccitt("123456789", 9) → 0x29B1
```

**Example 2** — edge cases
```
crc16_ccitt(NULL, 0)     → 0xFFFF   (empty returns init value)
crc16_ccitt(data, 0)     → 0xFFFF
```

**Example 3** — single bytes
```
crc16_ccitt({0x00}, 1)   → 0xE1F0
crc16_ccitt({0xFF}, 1)   → 0xFF00
```

## Constraints

- Bit-by-bit computation (no lookup table required)
- For each byte, process 8 bits MSB first
- For each bit: if MSB of CRC is 1, shift left and XOR with 0x1021; else just shift left

**Optimal complexity:** O(8n) — 8 bit iterations per byte
