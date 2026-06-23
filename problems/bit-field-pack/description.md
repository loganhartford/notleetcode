# Pack / Unpack a Bit-Field

Pack **two 4-bit values** (nibbles) into a single byte, and unpack them back out.
This is the bug that bit your Christmas-tree flash: getting the shift/mask wrong
when two fields share a byte.

```c
uint8_t pack_nibbles(uint8_t hi, uint8_t lo);  // hi -> high nibble, lo -> low nibble
uint8_t unpack_hi(uint8_t b);                  // the high 4 bits, as 0..15
uint8_t unpack_lo(uint8_t b);                  // the low 4 bits, as 0..15
```

- Only the low 4 bits of `hi` and `lo` are used; **mask off** anything above
  bit 3 so a too-big input can't bleed into the other field.
- `pack_nibbles(hi, lo)` returns `((hi & 0xF) << 4) | (lo & 0xF)`.

### Examples
```
pack_nibbles(0xA, 0x5) -> 0xA5
unpack_hi(0xA5)        -> 0xA
unpack_lo(0xA5)        -> 0x5

pack_nibbles(0x1, 0x2) -> 0x12
pack_nibbles(0xFF, 0x3) -> 0xF3   // hi is masked to 0xF
```

### Constraints
- Work on `uint8_t`.
- This is `[author]`-defined firmware practice.
