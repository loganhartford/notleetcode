#include <stdint.h>

int is_little_endian_layout(const void *one) {
    const uint8_t * p = (const uint8_t *)one; // take the pointer to the start of the memory
    return *p; // if the least significant byte is in the first byte, this will be 1
}

int is_little_endian(void) {
    return 0;
}

uint32_t to_network_order(uint32_t x, int host_is_little) {
    if (host_is_little) {

    } else {
        return x;
    }
}

uint32_t my_htonl(uint32_t x) {
    return 0;
}
