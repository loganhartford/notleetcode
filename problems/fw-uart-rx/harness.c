#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlc.h"
#include "solution.c"

static void chk_sz(const char *nm, const char *in, size_t got, size_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_mem(const char *nm, const char *in,
                    const uint8_t *got, const uint8_t *exp, size_t n) {
    int ok = memcmp(got, exp, n) == 0;
    char g[64]="",e[64]="";
    for (size_t i=0;i<n&&i<8;i++){
        char tmp[8];
        snprintf(tmp,sizeof tmp,"%02X ",got[i]); strncat(g,tmp,sizeof g-strlen(g)-1);
        snprintf(tmp,sizeof tmp,"%02X ",exp[i]); strncat(e,tmp,sizeof e-strlen(e)-1);
    }
    nlc_case(nm, ok, in, e, g, "");
}

int main(void) {
    nlc_begin();

    uint8_t storage[8];
    uart_rx_t u;
    uint8_t out[8];

    /* basic read */
    uart_rx_init(&u, storage, 8);
    uart_rx_isr_byte(&u, 'H');
    uart_rx_isr_byte(&u, 'i');
    size_t n = uart_rx_read(&u, out, 8);
    chk_sz("basic read count", "uart_rx_read 2 bytes", n, 2);
    chk_mem("basic read data", "read data", out, (uint8_t*)"Hi", 2);

    /* read from empty returns 0 */
    n = uart_rx_read(&u, out, 8);
    chk_sz("read empty=0", "uart_rx_read empty", n, 0);

    /* wraparound */
    uart_rx_init(&u, storage, 4);
    uart_rx_isr_byte(&u, 1); uart_rx_isr_byte(&u, 2);
    uart_rx_isr_byte(&u, 3); uart_rx_isr_byte(&u, 4);
    uart_rx_read(&u, out, 2); /* consume 1,2 */
    uart_rx_isr_byte(&u, 5); uart_rx_isr_byte(&u, 6); /* wrap */
    n = uart_rx_read(&u, out, 4);
    chk_sz("wraparound read count", "wraparound read", n, 4);
    uint8_t exp_wrap[] = {3,4,5,6};
    chk_mem("wraparound data", "wraparound data", out, exp_wrap, 4);

    /* overflow drops and counts */
    uart_rx_init(&u, storage, 3);
    uart_rx_isr_byte(&u, 10); uart_rx_isr_byte(&u, 20); uart_rx_isr_byte(&u, 30);
    uart_rx_isr_byte(&u, 40); /* dropped */
    uart_rx_isr_byte(&u, 50); /* dropped */
    chk_sz("dropped=2", "uart_rx_dropped after 2 drops", uart_rx_dropped(&u), 2);
    n = uart_rx_read(&u, out, 8);
    chk_sz("after overflow count=3", "read count after overflow", n, 3);
    uint8_t exp_ov[] = {10,20,30};
    chk_mem("overflow data intact", "overflow data", out, exp_ov, 3);

    /* partial read */
    uart_rx_init(&u, storage, 8);
    for (int i = 0; i < 6; i++) uart_rx_isr_byte(&u, (uint8_t)(i+1));
    n = uart_rx_read(&u, out, 3);
    chk_sz("partial read=3", "partial read max_len=3", n, 3);
    uint8_t exp_p[] = {1,2,3};
    chk_mem("partial read data", "partial data", out, exp_p, 3);
    n = uart_rx_read(&u, out, 8);
    chk_sz("remaining=3", "remaining after partial", n, 3);

    nlc_end();
    return 0;
}
