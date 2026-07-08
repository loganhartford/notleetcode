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


size_t sensor_msg_encode_v1(uint8_t *out, size_t out_len, const sensor_msg_t *msg) {
    if (out_len < SENSOR_MSG_WIRE_LEN)
    {
        return 0;
    }
    
    uint8_t *m = (uint8_t *)msg;
    
    // Debub prints
    fprintf(stderr, "%x\n", msg->device_id);
    fprintf(stderr, "msg: 0x");
    for (int i = 0; i < SENSOR_MSG_WIRE_LEN; i++) {
        fprintf(stderr, "%02x", *(m+i));
    }
    fprintf(stderr, "\n");
    
    /* I think this only works becuase the host happens
       to be little endian, but would break otherwise */
    size_t n = SENSOR_MSG_WIRE_LEN;
    while(n--) {
        *out++ = *m++;
    };
    
    /* Learning:
    - The value of device_id is 0x1020304
    - But becuase the host is little endian, it's stored in memory
      as 0x04030201, where LSB is first.
    - Wire formate is always little endian, so we can just output bits
      in the same ordered they are stored on the host
    */
    
    
    return SENSOR_MSG_WIRE_LEN;
}

size_t sensor_msg_encode(uint8_t *out, size_t out_len, const sensor_msg_t *msg) {
    if (out_len < SENSOR_MSG_WIRE_LEN)
    {
        return 0;
    }
    
    uint32_t id = msg->device_id;
    uint16_t tmp = (uint16_t)msg->temperature_q8_8;
    uint16_t mv = msg->millivolts;
    
    out[0] = (0xFF & id);
    out[1] = (0xFF & (id>>8));
    out[2] = (0xFF & (id>>16));
    out[3] = (0xFF & (id>>24));
    out[4] = (0xFF & (tmp));
    out[5] = (0xFF & (tmp>>8));
    out[6] = (0xFF & (mv));
    out[7] = (0xFF & (mv>>8));
    out[8] = (msg->flags);
    
    return SENSOR_MSG_WIRE_LEN;
}

bool sensor_msg_decode(sensor_msg_t *msg, const uint8_t *in, size_t in_len) {
    if (in_len < SENSOR_MSG_WIRE_LEN)
    {
        return false;
    }
    
    msg->device_id = ((in[3] << 24) |
                      (in[2] << 16) |
                      (in[1] << 8 ) |
                      (in[0]));
    msg->temperature_q8_8 = ((in[5] << 8) | in[4]);
    msg->millivolts =       ((in[7] << 8) | in[6]);
    msg->flags = in[8];
    
    return true;
}
