#include <stdint.h>
#include <stdbool.h>

void db_init(debounce_t *d, bool initial_level, uint32_t debounce_ms) {
    // TODO
}

db_event_t db_update(debounce_t *d, bool raw_level, uint32_t now_ms) {
    // TODO
    return DB_NONE;
}

bool db_level(const debounce_t *d) {
    // TODO
    return false;
}
