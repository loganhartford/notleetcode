#include <stdint.h>

int is_little_endian_layout(const void *one) {
    return *(const unsigned char *)one == 1;
}

int is_little_endian(void) {
    const uint32_t one = 1;
    return is_little_endian_layout(&one);
}

uint32_t to_network_order(uint32_t x, int host_is_little) {
    if (!host_is_little)
        return x;
    return ((x & 0x000000FFu) << 24) |
           ((x & 0x0000FF00u) <<  8) |
           ((x & 0x00FF0000u) >>  8) |
           ((x & 0xFF000000u) >> 24);
}

uint32_t my_htonl(uint32_t x) {
    return to_network_order(x, is_little_endian());
}
