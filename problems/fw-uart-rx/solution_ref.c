// Time: O(n) read, O(1) isr_byte
#include <stdint.h>
#include <stddef.h>


typedef struct {
    volatile uint8_t *buf;
    size_t            capacity;
    volatile size_t   head;
    volatile size_t   tail;
    volatile size_t   dropped;
} uart_rx_t;

void uart_rx_init(uart_rx_t *u, uint8_t *storage, size_t capacity) {
    u->buf      = storage;
    u->capacity = capacity;
    u->head     = 0;
    u->tail     = 0;
    u->dropped  = 0;
}

void uart_rx_isr_byte(uart_rx_t *u, uint8_t byte) {
    size_t head = u->head;
    if (head - u->tail == u->capacity) {
        u->dropped++;
        return;
    }
    u->buf[head % u->capacity] = byte;
    u->head = head + 1;
}

size_t uart_rx_read(uart_rx_t *u, uint8_t *out, size_t max_len) {
    size_t tail  = u->tail;
    size_t avail = u->head - tail;
    size_t n     = avail < max_len ? avail : max_len;
    for (size_t i = 0; i < n; i++) {
        out[i] = u->buf[(tail + i) % u->capacity];
    }
    u->tail = tail + n;
    return n;
}

size_t uart_rx_dropped(const uart_rx_t *u) {
    return u->dropped;
}
