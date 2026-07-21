#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_WORD_SIZE 5

volatile char UART_RX_REGISTER = '\0';

/* Declare whatever ring-buffer state you need here. */


void uart_init(void) {

}

/* Hardware fires this when a byte lands in UART_RX_REGISTER. Do not print here. */
void uart_receive_isr(void) {

}

/* Runs from the super-loop. Print whatever has arrived, words on their own lines. */
void main_loop(void) {

}
