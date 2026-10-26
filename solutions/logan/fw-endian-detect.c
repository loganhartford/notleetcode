#include <stdint.h>

int is_little_endian_layout(const void *one) {
    const uint8_t * p = (const uint8_t *)one; // take the pointer to the start of the memory
    return *p; // if the least significant byte is in the first byte, this will be 1
}

int is_little_endian(void) {
    uint16_t x = 1;
    uint8_t *p = (uint8_t *)&x;
    return *p;
}

uint32_t to_network_order(uint32_t x, int host_is_little) {
    if (host_is_little) {
        return ((x & 0x000000FF) << 24) | 
            ((x & 0x0000FF00) << 8) | 
            ((x & 0x00FF0000) >> 8) | 
            ((x & 0xFF000000) >> 24);
    } else {
        return x;
    }
}

uint32_t my_htonl(uint32_t x) {
    if (is_little_endian()) {
        return ((x & 0x000000FF) << 24) | 
            ((x & 0x0000FF00) << 8) | 
            ((x & 0x00FF0000) >> 8) | 
            ((x & 0xFF000000) >> 24);
    } else {
        return x;
    }
}
