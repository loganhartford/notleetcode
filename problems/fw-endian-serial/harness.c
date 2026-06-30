#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t device_id;
    int16_t  temperature_q8_8;
    uint16_t millivolts;
    uint8_t  flags;
} sensor_msg_t;
#define SENSOR_MSG_WIRE_LEN 9

#include "nlc.h"
#include "solution.c"

static void chk_sz(const char *nm, const char *in, size_t got, size_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp ? "true" : "false", got ? "true" : "false", "");
}
static void chk_byte(const char *nm, const char *in, uint8_t got, uint8_t exp) {
    char g[8], e[8];
    snprintf(g, sizeof g, "0x%02X", got);
    snprintf(e, sizeof e, "0x%02X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_u32(const char *nm, const char *in, uint32_t got, uint32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "0x%08X", got);
    snprintf(e, sizeof e, "0x%08X", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_i16(const char *nm, const char *in, int16_t got, int16_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", (int)got);
    snprintf(e, sizeof e, "%d", (int)exp);
    nlc_case(nm, got == exp, in, e, g, "");
}

int main(void) {
    nlc_begin();

    uint8_t buf[12];
    sensor_msg_t msg = { 0x01020304u, 0x0140, 0x0BB8, 0x05 };

    /* encode returns 9 */
    chk_sz("encode returns 9", "sensor_msg_encode", sensor_msg_encode(buf, 12, &msg), 9);

    /* verify little-endian byte order */
    chk_byte("device_id byte0", "buf[0]", buf[0], 0x04);
    chk_byte("device_id byte1", "buf[1]", buf[1], 0x03);
    chk_byte("device_id byte2", "buf[2]", buf[2], 0x02);
    chk_byte("device_id byte3", "buf[3]", buf[3], 0x01);
    chk_byte("temp byte0",      "buf[4]", buf[4], 0x40);
    chk_byte("temp byte1",      "buf[5]", buf[5], 0x01);
    chk_byte("mv byte0",        "buf[6]", buf[6], 0xB8);
    chk_byte("mv byte1",        "buf[7]", buf[7], 0x0B);
    chk_byte("flags",           "buf[8]", buf[8], 0x05);

    /* round-trip */
    sensor_msg_t dec;
    chk_bool("decode ok", "sensor_msg_decode", sensor_msg_decode(&dec, buf, 9), true);
    chk_u32("rt device_id", "decoded device_id", dec.device_id, 0x01020304u);
    chk_i16("rt temp",      "decoded temp",      dec.temperature_q8_8, 0x0140);
    chk_byte("rt flags",    "decoded flags",      dec.flags, 0x05);

    /* negative temperature round-trip */
    sensor_msg_t msg2 = { 1, -256, 3300, 0xFF };
    sensor_msg_encode(buf, 12, &msg2);
    sensor_msg_decode(&dec, buf, 9);
    chk_i16("negative temp rt", "decoded negative temp", dec.temperature_q8_8, -256);

    /* truncated decode fails */
    chk_bool("decode truncated", "sensor_msg_decode short", sensor_msg_decode(&dec, buf, 8), false);

    /* truncated encode returns 0 */
    chk_sz("encode buf too small", "sensor_msg_encode short", sensor_msg_encode(buf, 8, &msg), 0);

    nlc_end();
    return 0;
}
