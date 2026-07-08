// Time: O(1) per sample — simple state checks
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
    d->stable        = initial_level;
    d->pending       = initial_level;
    d->pending_start = 0;
    d->debounce_ms   = debounce_ms;
}

db_event_t db_update(debounce_t *d, bool raw_level, uint32_t now_ms) {
    if (raw_level == d->stable) {
        /* Signal back to stable — reset any pending */
        d->pending = d->stable;
        return DB_NONE;
    }
    /* raw differs from stable */
    if (d->pending == d->stable) {
        /* New transition starting */
        d->pending       = raw_level;
        d->pending_start = now_ms;
        return DB_NONE;
    }
    /* Already tracking a transition */
    if ((uint32_t)(now_ms - d->pending_start) >= d->debounce_ms) {
        d->stable  = raw_level;
        d->pending = raw_level;
        return raw_level ? DB_RISING : DB_FALLING;
    }
    return DB_NONE;
}

bool db_level(const debounce_t *d) {
    return d->stable;
}
