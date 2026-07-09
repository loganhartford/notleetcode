#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

typedef struct {
    uint8_t *buf;
    size_t   capacity;
    size_t   head;
    size_t   count;
    size_t   dropped;
    pthread_mutex_t lock;
} uart_rx_t;



void uart_rx_init(uart_rx_t *u, uint8_t *storage, size_t capacity) {
    u->buf = storage;
    u->capacity = capacity;
    u->head = 0;
    u->count = 0;
    u->dropped = 0;
    pthread_mutex_init(&u->lock, NULL); // not checking return
}

void uart_rx_isr_byte(uart_rx_t *u, uint8_t byte) {
    if (u->count >= u->capacity)
    {
        u->dropped++;
        return;
    }
    uint8_t wrt = (u->head + u->count) % u->capacity;
    u->buf[wrt] = byte;
    pthread_mutex_lock(&u->lock);
    u->count++;
    pthread_mutex_unlock(&u->lock);
}

size_t uart_rx_read(uart_rx_t *u, uint8_t *out, size_t max_len) {
    size_t n = u->count < max_len ? u->count : max_len;
    printf("n: %u\n", n);
    for (size_t i = 0; i < n; i++) {
        out[i] = u->buf[u->head];
        u->head  = (u->head + 1) % u->capacity;
        pthread_mutex_lock(&u->lock);
        u->count--;
        pthread_mutex_unlock(&u->lock);
        printf("h: %u, c: %u\n", u->head, u->count);
    }
    return n;
}

size_t uart_rx_dropped(const uart_rx_t *u) {
    return u->dropped;
}
