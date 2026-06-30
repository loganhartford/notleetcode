// Time: O(n) read, O(1) isr_byte
#include <stdint.h>
#include <stddef.h>

void uart_rx_init(uart_rx_t *u, uint8_t *storage, size_t capacity) {
    u->buf      = storage;
    u->capacity = capacity;
    u->head     = 0;
    u->count    = 0;
    u->dropped  = 0;
}

void uart_rx_isr_byte(uart_rx_t *u, uint8_t byte) {
    if (u->count >= u->capacity) {
        u->dropped++;
        return;
    }
    size_t write_idx = (u->head + u->count) % u->capacity;
    u->buf[write_idx] = byte;
    u->count++;
}

size_t uart_rx_read(uart_rx_t *u, uint8_t *out, size_t max_len) {
    size_t n = u->count < max_len ? u->count : max_len;
    for (size_t i = 0; i < n; i++) {
        out[i] = u->buf[(u->head + i) % u->capacity];
    }
    u->head  = (u->head + n) % u->capacity;
    u->count -= n;
    return n;
}

size_t uart_rx_dropped(const uart_rx_t *u) {
    return u->dropped;
}
