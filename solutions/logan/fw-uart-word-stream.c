#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_WORD_SIZE 5

volatile char UART_RX_REGISTER = '\0';

/* Declare whatever ring-buffer state you need here. */
typedef struct {
    char data[MAX_WORD_SIZE];
    size_t capacity;
    uint8_t head;
    uint8_t tail;
} circular_buffer_t ;

circular_buffer_t ring = {
    .capacity = MAX_WORD_SIZE,
    .head = 0,
    .tail = 0,
};

void uart_init(void) {
    ring.head = 0;
    ring.tail = 0;
    for (int i = 0; i < ring.capacity; i++) {
        ring.data[i] = 0;
    }
}

/* Hardware fires this when a byte lands in UART_RX_REGISTER. Do not print here. */
void uart_receive_isr(void) {
    ring.data[ring.head] = UART_RX_REGISTER;
    ring.head = (ring.head + 1) % ring.capacity;
}

/* Runs from the super-loop. Print whatever has arrived, words on their own lines. */
void main_loop(void) {
    while (ring.tail != ring.head) {
        if (ring.data[ring.tail] == ' ') {
            printf("\n");
        } else {
            printf("%c", ring.data[ring.tail]);
        }
        ring.tail = (ring.tail + 1) % ring.capacity;
    }
}
