#include <stdint.h>

static uint64_t load_frame(const uint8_t frame[8]) {
    uint64_t raw = 0;
    for (int i = 7; i >= 0; i--)
        raw = (raw << 8) | frame[i];
    return raw;
}

uint32_t can_get_signal(const uint8_t frame[8], unsigned start_bit, unsigned len) {
    uint64_t mask = (1ULL << len) - 1;
    return (uint32_t)((load_frame(frame) >> start_bit) & mask);
}

int32_t can_get_signed(const uint8_t frame[8], unsigned start_bit, unsigned len) {
    int64_t value = can_get_signal(frame, start_bit, len);
    if (value >> (len - 1))
        value -= (int64_t)1 << len;
    return (int32_t)value;
}

void can_set_signal(uint8_t frame[8], unsigned start_bit, unsigned len, uint32_t value) {
    uint64_t mask = ((1ULL << len) - 1) << start_bit;
    uint64_t raw = (load_frame(frame) & ~mask) | (((uint64_t)value << start_bit) & mask);
    for (int i = 0; i < 8; i++) {
        frame[i] = (uint8_t)raw;
        raw >>= 8;
    }
}
