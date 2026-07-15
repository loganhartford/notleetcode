#include <stdio.h>
#include <stdbool.h>

#define CB_CAPACITY 8

typedef struct {
    int  data[CB_CAPACITY];
    int  head;    /* index of the front element           */
    int  tail;    /* index where the next push will write */
    int  count;   /* current number of elements           */
} CircularBuffer;

void cb_init(CircularBuffer *cb) {
    cb->head  = 0;
    cb->tail  = 0;
    cb->count = 0;
}

int cb_size(const CircularBuffer *cb) {
    return 0;
}

bool cb_is_empty(const CircularBuffer *cb) {
    return true;
}

bool cb_is_full(const CircularBuffer *cb) {
    return false;
}

int cb_push(CircularBuffer *cb, int value) {
    return -1;
}

int cb_pop(CircularBuffer *cb, int *out) {
    return -1;
}

int cb_peek(const CircularBuffer *cb, int *out) {
    return -1;
}
