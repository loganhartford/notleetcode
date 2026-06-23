typedef struct {
    int stable;
    int candidate;
    int count;
    int threshold;
} debounce_t;

void debounce_init(debounce_t* db, int initial, int threshold) {
    db->stable = initial;
    db->candidate = initial;
    db->count = 0;
    db->threshold = threshold;
}

int debounce_update(debounce_t* db, int raw) {
    if (raw == db->stable) {
        db->count = 0;
    } else {
        if (raw == db->candidate) {
            db->count++;
        } else {
            db->candidate = raw;
            db->count = 1;
        }
        if (db->count >= db->threshold) {
            db->stable = raw;
            db->count = 0;
        }
    }
    return db->stable;
}
