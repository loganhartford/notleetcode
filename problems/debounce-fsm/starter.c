typedef struct {
    int stable;
    int candidate;
    int count;
    int threshold;
} debounce_t;

void debounce_init(debounce_t* db, int initial, int threshold) {
}

int debounce_update(debounce_t* db, int raw) {
    return 0;
}
