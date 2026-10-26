# CAN Signal Pack and Unpack

A classic CAN frame carries 8 data bytes. A DBC file describes each signal in that payload (a wheel speed, a temperature, a status flag) by a start bit and a length in bits; a signal need not be byte-aligned and can span several bytes. Implement reading and writing one signal in Intel (little-endian) layout, the `@1` byte order in a DBC file.

```c
uint32_t can_get_signal(const uint8_t frame[8], unsigned start_bit, unsigned len);
int32_t  can_get_signed(const uint8_t frame[8], unsigned start_bit, unsigned len);
void     can_set_signal(uint8_t frame[8], unsigned start_bit, unsigned len, uint32_t value);
```

Bit `k` of the frame is bit `k % 8` of `frame[k / 8]`: bit 0 is the least significant bit of `frame[0]` and bit 63 is the most significant bit of `frame[7]`. A signal occupies frame bits `start_bit` through `start_bit + len - 1`, and its least significant bit is at `start_bit`.

The signal with `start_bit = 4` and `len = 12`, across bytes 0 to 2:

```
                  frame[0]                  frame[1]                  frame[2]
frame bit   7  6  5  4  3  2  1  0 | 15 14 13 12 11 10  9  8 | 23 22 21 20 19 18 17 16
signal bit  3  2  1  0  .  .  .  . | 11 10  9  8  7  6  5  4 |  .  .  .  .  .  .  .  .
```

- `can_get_signal` returns the signal's raw unsigned value.
- `can_get_signed` returns the signal's value read as a `len`-bit two's complement number.
- `can_set_signal` writes the low `len` bits of `value` into the signal. Bits of `value` above `len` are ignored, and every frame bit outside the signal keeps its value.

### Example 1
```
Input:  frame = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0}
        can_get_signal(frame, 16, 8)
Output: 0x56
```

### Example 2
```
Input:  frame = {0xAB, 0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
        can_get_signal(frame, 4, 12)
Output: 0xCDA
Explanation: signal bits 0-3 are frame[0] bits 4-7 (0xA),
             signal bits 4-11 are frame[1] bits 0-7 (0xCD).
```

### Example 3
```
Input:  frame = {0x0F, 0x80, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00}
        can_get_signal(frame, 4, 12)
        can_get_signed(frame, 4, 12)
Output: 0x800
        -2048
```

### Example 4
```
Input:  frame = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
        can_set_signal(frame, 4, 12, 0x123)
Output: frame = {0x3F, 0x12, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
```

### Example 5
```
Input:  frame = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
        can_set_signal(frame, 10, 4, 0xFFFFFFF5)
Output: frame = {0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
Explanation: only the low 4 bits of value (0x5) are written, into frame bits 10-13.
```

### Constraints
- `1 <= len <= 32` and `start_bit + len <= 64`; the caller guarantees both
- `can_set_signal` must not change any frame bit outside `start_bit .. start_bit + len - 1`
- Python: `can_get_signal(frame: bytes, start_bit: int, length: int) -> int` and `can_get_signed` (same arguments, returns a negative `int` when the value is negative) read the frame; `can_set_signal(frame: bytearray, start_bit: int, length: int, value: int) -> None` modifies `frame` in place, with `0 <= value < 2**32`

**Optimal complexity:** O(1) time and O(1) space per call
