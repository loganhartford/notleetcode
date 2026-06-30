// Time: O(1) — simple bounds/alignment checks then byte-level store/load
#include <stdint.h>
#include <stdbool.h>

static int mmio_valid(uintptr_t addr) {
    return (addr % 4 == 0) &&
           (addr >= MMIO_BASE) &&
           (addr + 4 <= MMIO_BASE + MMIO_SIZE);
}

bool mmio_write32(uintptr_t addr, uint32_t value) {
    if (!mmio_valid(addr)) return false;
    size_t off = addr - MMIO_BASE;
    _fake_mem[off + 0] = (uint8_t)(value       & 0xFF);
    _fake_mem[off + 1] = (uint8_t)((value >> 8) & 0xFF);
    _fake_mem[off + 2] = (uint8_t)((value >>16) & 0xFF);
    _fake_mem[off + 3] = (uint8_t)((value >>24) & 0xFF);
    return true;
}

bool mmio_read32(uintptr_t addr, uint32_t *out) {
    if (!mmio_valid(addr)) return false;
    size_t off = addr - MMIO_BASE;
    *out = (uint32_t)_fake_mem[off + 0]
         | ((uint32_t)_fake_mem[off + 1] <<  8)
         | ((uint32_t)_fake_mem[off + 2] << 16)
         | ((uint32_t)_fake_mem[off + 3] << 24);
    return true;
}
