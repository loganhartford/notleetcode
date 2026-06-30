#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk_mem(const char *nm, const char *in, const uint8_t *got, const uint8_t *exp, size_t n) {
    int ok = memcmp(got, exp, n) == 0;
    char g[64] = "", e[64] = "";
    for (size_t i = 0; i < n && i < 8; i++) {
        char tmp[8];
        snprintf(tmp, sizeof tmp, "%02X ", got[i]); strncat(g, tmp, sizeof g - strlen(g) - 1);
        snprintf(tmp, sizeof tmp, "%02X ", exp[i]); strncat(e, tmp, sizeof e - strlen(e) - 1);
    }
    nlc_case(nm, ok, in, e, g, "");
}

int main(void) {
    nlc_begin();

    uint8_t src[8] = {1,2,3,4,5,6,7,8};
    uint8_t dst[8];

    /* memcpy zero-length */
    memset(dst, 0xFF, sizeof dst);
    c_memcpy(dst, src, 0);
    uint8_t ff[8]; memset(ff, 0xFF, 8);
    chk_mem("memcpy zero-len", "c_memcpy(dst,src,0)", dst, ff, 8);

    /* memcpy one byte */
    memset(dst, 0, sizeof dst);
    c_memcpy(dst, src, 1);
    uint8_t e1[8] = {1,0,0,0,0,0,0,0};
    chk_mem("memcpy 1 byte", "c_memcpy(dst,src,1)", dst, e1, 8);

    /* memcpy all */
    memset(dst, 0, sizeof dst);
    c_memcpy(dst, src, 8);
    chk_mem("memcpy 8 bytes", "c_memcpy(dst,src,8)", dst, src, 8);

    /* memmove overlapping dst>src (forward overlap) */
    uint8_t buf[8] = {1,2,3,4,5,0,0,0};
    c_memmove(buf + 2, buf, 4);
    uint8_t em1[8] = {1,2,1,2,3,4,0,0};
    chk_mem("memmove dst>src overlap", "c_memmove(buf+2,buf,4)", buf, em1, 8);

    /* memmove overlapping dst<src (backward overlap) */
    uint8_t buf2[8] = {0,0,1,2,3,4,0,0};
    c_memmove(buf2, buf2 + 2, 4);
    uint8_t em2[8] = {1,2,3,4,3,4,0,0};
    chk_mem("memmove dst<src overlap", "c_memmove(buf2,buf2+2,4)", buf2, em2, 8);

    /* memset to 0 */
    memset(dst, 0xFF, sizeof dst);
    c_memset(dst, 0, 8);
    uint8_t z[8] = {0};
    chk_mem("memset 0x00", "c_memset(dst,0,8)", dst, z, 8);

    /* memset to 0xFF */
    memset(dst, 0, sizeof dst);
    c_memset(dst, 0xFF, 8);
    chk_mem("memset 0xFF", "c_memset(dst,0xFF,8)", dst, ff, 8);

    /* memset to 0x55 */
    uint8_t e55[8]; memset(e55, 0x55, 8);
    c_memset(dst, 0x55, 8);
    chk_mem("memset 0x55", "c_memset(dst,0x55,8)", dst, e55, 8);

    nlc_end();
    return 0;
}
