# GPIO Debouncer with Timestamps

Implement a software GPIO debouncer that filters mechanical button bounce using timestamps. The debouncer tracks a pending transition and only commits it if the raw signal holds steady for at least `debounce_ms` milliseconds.

The `debounce_t` struct and `db_event_t` enum are provided. Implement:

```c
void       db_init(debounce_t *d, bool initial_level, uint32_t debounce_ms);
db_event_t db_update(debounce_t *d, bool raw_level, uint32_t now_ms);
bool       db_level(const debounce_t *d);
```

**Logic:**
1. If `raw_level != stable`, start tracking: set `pending = raw_level`, `pending_start = now_ms`
2. If raw is still different from stable AND `(uint32_t)(now_ms - pending_start) >= debounce_ms`, commit: update `stable`, return `DB_RISING` or `DB_FALLING`
3. If `raw_level == stable` while pending, reset the pending state
4. Otherwise return `DB_NONE`

## Examples

**Example 1** — clean rising edge
```
db_init(&d, false, 10);
db_update(&d, true, 0);   → DB_NONE (start tracking)
db_update(&d, true, 9);   → DB_NONE (not yet)
db_update(&d, true, 10);  → DB_RISING (debounce_ms elapsed)
db_level(&d)              → true
```

**Example 2** — bounce that resets
```
db_init(&d, false, 10);
db_update(&d, true, 0);   → DB_NONE (start tracking)
db_update(&d, false, 5);  → DB_NONE (bounced back — reset)
db_update(&d, true, 6);   → DB_NONE (restart tracking)
db_update(&d, true, 16);  → DB_RISING
```

**Example 3** — no change
```
db_init(&d, true, 10);
db_update(&d, true, 100); → DB_NONE (no change)
db_level(&d)              → true
```

## Constraints

- Use `(uint32_t)(now_ms - pending_start) >= debounce_ms` for the time check (wraparound-safe)
- Only return an event on the poll that crosses the debounce threshold; subsequent polls with the same stable level return `DB_NONE`
- `db_level` returns the committed stable level, not the raw level

**Optimal complexity:** O(1) per sample
