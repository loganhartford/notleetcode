#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_WORD_SIZE 5
#define RX_BUF_SIZE   8   /* room for the (up to 3) pending bytes, plus one empty slot */

volatile char UART_RX_REGISTER = '\0';

/* Single-producer (ISR advances head) / single-consumer (main advances tail) ring.
 * Because each index is written by only one context, no locking is required. */
static volatile char     rx_buf[RX_BUF_SIZE];
static volatile uint32_t rx_head = 0;
static volatile uint32_t rx_tail = 0;

void uart_init(void) {
    rx_head = 0;
    rx_tail = 0;
}

void uart_receive_isr(void) {
    uint32_t next = (rx_head + 1) % RX_BUF_SIZE;
    if (next != rx_tail) {                 /* if full, drop rather than corrupt */
        rx_buf[rx_head] = UART_RX_REGISTER;
        rx_head = next;
    }
}

void main_loop(void) {
    while (rx_tail != rx_head) {
        char c = rx_buf[rx_tail];
        putchar(c == ' ' ? '\n' : c);
        rx_tail = (rx_tail + 1) % RX_BUF_SIZE;
    }
}
