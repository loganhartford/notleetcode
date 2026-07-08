// Time: O(1) — fixed-size message, explicit byte-by-byte serialization
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
    if (out_len < SENSOR_MSG_WIRE_LEN) return 0;
    uint32_t id  = msg->device_id;
    uint16_t tmp = (uint16_t)msg->temperature_q8_8;
    uint16_t mv  = msg->millivolts;
    out[0] = (uint8_t)(id  & 0xFF);
    out[1] = (uint8_t)((id  >>  8) & 0xFF);
    out[2] = (uint8_t)((id  >> 16) & 0xFF);
    out[3] = (uint8_t)((id  >> 24) & 0xFF);
    out[4] = (uint8_t)(tmp & 0xFF);
    out[5] = (uint8_t)((tmp >>  8) & 0xFF);
    out[6] = (uint8_t)(mv  & 0xFF);
    out[7] = (uint8_t)((mv  >>  8) & 0xFF);
    out[8] = msg->flags;
    return SENSOR_MSG_WIRE_LEN;
}

bool sensor_msg_decode(sensor_msg_t *msg, const uint8_t *in, size_t in_len) {
    if (in_len < SENSOR_MSG_WIRE_LEN) return false;
    msg->device_id = (uint32_t)in[0]
                   | ((uint32_t)in[1] <<  8)
                   | ((uint32_t)in[2] << 16)
                   | ((uint32_t)in[3] << 24);
    uint16_t tmp = (uint16_t)in[4] | ((uint16_t)in[5] << 8);
    msg->temperature_q8_8 = (int16_t)tmp;
    msg->millivolts = (uint16_t)in[6] | ((uint16_t)in[7] << 8);
    msg->flags = in[8];
    return true;
}
