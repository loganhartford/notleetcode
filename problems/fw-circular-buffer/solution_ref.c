#include <stdio.h>
#include <stdbool.h>

#define CB_CAPACITY 8

typedef struct {
    int  data[CB_CAPACITY];
    int  head;
    int  tail;
    int  count;
} CircularBuffer;

void cb_init(CircularBuffer *cb) {
    cb->head  = 0;
    cb->tail  = 0;
    cb->count = 0;
}

int cb_size(const CircularBuffer *cb) {
    return cb->count;
}

bool cb_is_empty(const CircularBuffer *cb) {
    return cb->count == 0;
}

bool cb_is_full(const CircularBuffer *cb) {
    return cb->count == CB_CAPACITY;
}

int cb_push(CircularBuffer *cb, int value) {
    if (cb_is_full(cb)) return -1;
    cb->data[cb->tail] = value;
    cb->tail = (cb->tail + 1) % CB_CAPACITY;
    cb->count++;
    return 0;
}

int cb_pop(CircularBuffer *cb, int *out) {
    if (cb_is_empty(cb)) return -1;
    *out = cb->data[cb->head];
    cb->head = (cb->head + 1) % CB_CAPACITY;
    cb->count--;
    return 0;
}

int cb_peek(const CircularBuffer *cb, int *out) {
    if (cb_is_empty(cb)) return -1;
    *out = cb->data[cb->head];
    return 0;
}
