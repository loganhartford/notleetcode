#include <stdint.h>
#include <stddef.h>


typedef struct {
    uint8_t *buf;
    size_t   capacity;
    size_t   head;
    size_t   count;
    size_t   dropped;
} uart_rx_t;

void uart_rx_init(uart_rx_t *u, uint8_t *storage, size_t capacity) {
    // TODO
}

void uart_rx_isr_byte(uart_rx_t *u, uint8_t byte) {
    // TODO
}

size_t uart_rx_read(uart_rx_t *u, uint8_t *out, size_t max_len) {
    // TODO
    return 0;
}

size_t uart_rx_dropped(const uart_rx_t *u) {
    // TODO
    return 0;
}
