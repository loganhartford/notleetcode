#include <stdint.h>

void *byte_offset(void *base, size_t offset) {
    return (uint8_t *)base + offset;
}
