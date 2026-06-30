# MMIO Read/Write with Validation

Implement memory-mapped I/O (MMIO) access functions that validate addresses before accessing hardware registers. In real firmware, accessing an unaligned or out-of-range address would cause a bus fault.

The fake MMIO region is defined as a `uint8_t` array `_fake_mem[MMIO_SIZE]` accessible at addresses `[MMIO_BASE, MMIO_BASE + MMIO_SIZE)`. These are defined in the harness.

```c
bool mmio_write32(uintptr_t addr, uint32_t value);
bool mmio_read32(uintptr_t addr, uint32_t *out);
```

Reject (return `false`) if:
- `addr` is not 4-byte aligned (`addr % 4 != 0`)
- `addr < MMIO_BASE`
- `addr + 4 > MMIO_BASE + MMIO_SIZE`

Otherwise, store/load as **little-endian** bytes in `_fake_mem[addr - MMIO_BASE]`.

## Examples

**Example 1** — aligned write and read
```
mmio_write32(0x1000, 0x12345678) → true
mmio_read32(0x1000, &v)         → true, v=0x12345678
_fake_mem[0..3] = {0x78, 0x56, 0x34, 0x12}  (little-endian)
```

**Example 2** — boundary checks
```
mmio_write32(0x0FFF, 0) → false  (addr < MMIO_BASE)
mmio_write32(0x10FC, 0) → true   (last valid: 0x1000+0xFC, 4 bytes fit)
mmio_write32(0x1100, 0) → false  (addr+4 > MMIO_BASE+MMIO_SIZE)
```

**Example 3** — alignment
```
mmio_write32(0x1001, 0) → false  (not 4-byte aligned)
mmio_write32(0x1002, 0) → false  (not 4-byte aligned)
mmio_write32(0x1004, 0) → true   (aligned)
```

## Constraints

- `MMIO_BASE = 0x1000`, `MMIO_SIZE = 0x100`
- Store as little-endian: byte 0 = LSB, byte 3 = MSB
- `uintptr_t` for addresses (from `<stdint.h>`)
- `_fake_mem`, `MMIO_BASE`, and `MMIO_SIZE` are defined in the harness; your solution uses them directly

**Optimal complexity:** O(1)
