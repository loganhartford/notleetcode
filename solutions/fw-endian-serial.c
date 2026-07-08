#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    uint32_t device_id;
    int16_t  temperature_q8_8;
    uint16_t millivolts;
    uint8_t  flags;
} sensor_msg_t;
#define SENSOR_MSG_WIRE_LEN 9


size_t sensor_msg_encode(uint8_t *out, size_t out_len, const sensor_msg_t *msg) {
    if (out_len < SENSOR_MSG_WIRE_LEN)
    {
        return 0;
    }
    
    uint8_t *m = (uint8_t *)msg;
    
    size_t n = sizeof(msg->device_id);
    m += n;
    while(n--) {
        *out++ = *--m;
        fprintf(stderr, "%d", *m);
        };
    
    n = sizeof(msg->temperature_q8_8);
    m += n;
    while(n--) *out++ = *--m;
    
    n = sizeof(msg->millivolts);
    m += n;
    while(n--) *out++ = *--m;
    
    n = sizeof(msg->flags);
    m += n;
    while(n--) *out++ = *--m;
    
    
    return SENSOR_MSG_WIRE_LEN;
}

bool sensor_msg_decode(sensor_msg_t *msg, const uint8_t *in, size_t in_len) {
    // TODO
    return false;
}
