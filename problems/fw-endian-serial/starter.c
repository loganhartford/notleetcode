#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct {
    uint32_t device_id;
    int16_t  temperature_q8_8;
    uint16_t millivolts;
    uint8_t  flags;
} sensor_msg_t;
#define SENSOR_MSG_WIRE_LEN 9

size_t sensor_msg_encode(uint8_t *out, size_t out_len, const sensor_msg_t *msg) {
    // TODO
    return 0;
}

bool sensor_msg_decode(sensor_msg_t *msg, const uint8_t *in, size_t in_len) {
    // TODO
    return false;
}
