#include <stdint.h>

int is_little_endian(void) {
    const uint32_t one = 1;
    return *(const unsigned char *)&one == 1;
}

uint32_t my_htonl(uint32_t x) {
    if (!is_little_endian())
        return x;
    return ((x & 0x000000FFu) << 24) |
           ((x & 0x0000FF00u) <<  8) |
           ((x & 0x00FF0000u) >>  8) |
           ((x & 0xFF000000u) >> 24);
}
