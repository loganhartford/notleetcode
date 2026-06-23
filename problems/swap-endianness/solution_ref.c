#include <stdint.h>

uint32_t swap_endianness(uint32_t x) {
    return ((x >> 24) & 0x000000FFu) |
           ((x >> 8)  & 0x0000FF00u) |
           ((x << 8)  & 0x00FF0000u) |
           ((x << 24) & 0xFF000000u);
}
