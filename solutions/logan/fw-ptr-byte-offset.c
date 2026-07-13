#include <stdint.h>

void *byte_offset(void *base, size_t offset) {
    uint8_t * new_base = (uint8_t *)base;
    new_base += offset;
    return new_base;
}
