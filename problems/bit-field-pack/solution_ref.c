#include <stdint.h>

uint8_t pack_nibbles(uint8_t hi, uint8_t lo) {
    return (uint8_t)(((hi & 0x0F) << 4) | (lo & 0x0F));
}

uint8_t unpack_hi(uint8_t b) {
    return (uint8_t)((b >> 4) & 0x0F);
}

uint8_t unpack_lo(uint8_t b) {
    return (uint8_t)(b & 0x0F);
}
