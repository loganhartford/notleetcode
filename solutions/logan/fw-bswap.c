#include <stdint.h>

uint16_t bswap16_u(uint16_t x) {
    
    return (x << 8)|(x >> 8);
}

uint32_t bswap32_u(uint32_t x) {
    
    return (x << 24)|((x & 0x0000FF00) << 8)|((x & 0x00FF0000) >> 8)| (x>>24);
}
