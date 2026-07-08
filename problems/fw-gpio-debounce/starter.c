#include <stdint.h>
#include <stdbool.h>


typedef enum { DB_NONE=0, DB_RISING, DB_FALLING } db_event_t;
typedef struct {
    bool     stable;
    bool     pending;
    uint32_t pending_start;
    uint32_t debounce_ms;
} debounce_t;

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
