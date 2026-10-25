#include <stdint.h>

#define SECONDS_PER_YEAR (60UL * 60UL * 24UL * 365UL)

#define MIN(A, B) ((A) <= (B) ? (A) : (B))

#define WRITE_REG16(hi, lo, val)            \
    do {                                    \
        uint16_t v16_ = (uint16_t)(val);    \
        (hi) = (uint8_t)(v16_ >> 8);        \
        (lo) = (uint8_t)(v16_ & 0xFFu);     \
    } while (0)
