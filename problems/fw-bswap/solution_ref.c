// Time: O(1) — fixed number of shifts and masks
#include <stdint.h>

uint16_t bswap16_u(uint16_t x) {
    return (uint16_t)(((x & 0x00FFu) << 8) |
                      ((x & 0xFF00u) >> 8));
}

uint32_t bswap32_u(uint32_t x) {
    return ((x & 0x000000FFu) << 24) |
           ((x & 0x0000FF00u) <<  8) |
           ((x & 0x00FF0000u) >>  8) |
           ((x & 0xFF000000u) >> 24);
}
