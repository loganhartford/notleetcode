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
    
    out[0] = (msg->device_id >> 0) & 0xFF;
    out[1] = (msg->device_id >> 8) & 0xFF;
    out[2] = (msg->device_id >> 16) & 0xFF;
    out[3] = (msg->device_id >> 24) & 0xFF;
    
    out[4] = (msg->temperature_q8_8 >> 0) & 0xFF;
    out[5] = (msg->temperature_q8_8 >> 8) & 0xFF;
    
    out[6] = (msg->millivolts >> 0) & 0xFF;
    out[7] = (msg->millivolts >> 8) & 0xFF;
    
    out[8] = msg->flags;
    
    return SENSOR_MSG_WIRE_LEN;
}

bool sensor_msg_decode(sensor_msg_t *msg, const uint8_t *in, size_t in_len) {
    if (in_len < SENSOR_MSG_WIRE_LEN) return false;
    
    msg->device_id = in[0] | in[1] << 8 | in[2] << 16 | in[3] << 24;
    msg->temperature_q8_8 = (int16_t)(in[4] | in[5] << 8);
    msg->millivolts = in[6] | in[7] << 8;
    msg->flags = in[8];
    
    return true;
}
