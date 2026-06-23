# CRC8 / Checksum

Implement a byte-wise **CRC-8** over a buffer.

```c
uint8_t crc8(const uint8_t* data, size_t len);
```

Use exactly this variant so results are reproducible (CRC-8, polynomial `0x07`,
initial value `0x00`, no input/output reflection):

```
crc = 0
for each byte b in data:
    crc ^= b
    repeat 8 times:
        if crc & 0x80:  crc = (crc << 1) ^ 0x07
        else:           crc = crc << 1
return crc        # keep it in 8 bits
```

### Example
```
crc8("123456789", 9)  -> 0xF4     # the standard CRC-8 check value
crc8("", 0)           -> 0x00
crc8({0x00}, 1)       -> 0x00
```

### Constraints
- Mask back to 8 bits after the shifts (`crc & 0xFF`).
- `[author]`-defined firmware practice.
