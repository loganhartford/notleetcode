# Fixed-Capacity Hash Map

Implement a hash map from `uint32_t` keys to `uint32_t` values for a system with no heap. The map holds at most `HT_CAP` entries, and all of its state lives in the `ht_t` the caller passes in, using the struct's own arrays: no `malloc`, no static or global storage. The struct layout is fixed. How you use `state[i]` and `count` is up to you, and so is the hash function.

```c
#define HT_CAP 64

typedef struct {
    uint32_t keys[HT_CAP];
    uint32_t vals[HT_CAP];
    uint8_t  state[HT_CAP];
    uint32_t count;
} ht_t;

void ht_init(ht_t *t);
bool ht_put(ht_t *t, uint32_t key, uint32_t val);
bool ht_get(const ht_t *t, uint32_t key, uint32_t *val);
bool ht_del(ht_t *t, uint32_t key);
```

- **`ht_init`** makes `t` an empty map. `t` may contain garbage bytes, or be a map that was already in use.
- **`ht_put`** inserts `key` with `val`, or replaces the value if `key` is already present, and returns `true`. It returns `false` only when `key` is absent and the map already holds `HT_CAP` entries; the map is then unchanged.
- **`ht_get`** writes the value to `*val` and returns `true` if `key` is present. Otherwise it returns `false` and leaves `*val` untouched.
- **`ht_del`** removes `key` and returns `true` if it was present, `false` if not.

Every `uint32_t` is a valid key, including `0` and `0xFFFFFFFF`. All four operations must stay correct after any sequence of puts and deletes, however long it runs.

Python twin: the same four functions take a `HashTable` object (defined in the starter) whose `keys`, `vals` and `state` lists have fixed length `HT_CAP`; `ht_get(t, key)` returns the value, or `None` when the key is absent.

### Example 1
```
ht_init(&t);
ht_put(&t, 5, 50)      -> true
ht_put(&t, 69, 690)    -> true
ht_get(&t, 69, &v)     -> true, v = 690
ht_get(&t, 7, &v)      -> false, v unchanged
```

### Example 2
```
ht_put(&t, 5, 50)      -> true
ht_put(&t, 5, 51)      -> true     (value replaced; still one entry)
ht_get(&t, 5, &v)      -> true, v = 51
ht_del(&t, 5)          -> true
ht_del(&t, 5)          -> false
ht_get(&t, 5, &v)      -> false
```

### Example 3
```
put 64 distinct keys           -> all true
ht_put(&t, new_key, 1)         -> false    (full)
ht_put(&t, existing_key, 2)    -> true     (replacing a value needs no room)
ht_del(&t, existing_key)       -> true
ht_put(&t, new_key, 1)         -> true
```

### Constraints
- At most `HT_CAP` = 64 live entries
- Keys and values are any `uint32_t` (Python: any int in `[0, 2^32)`); no key value is reserved
- No `malloc` and no static or global variables: two `ht_t`s must be fully independent
- Python: no `dict`, `set`, or other built-in hashed container, and the three lists keep length `HT_CAP`

**Optimal complexity:** O(1) average time per operation, no memory beyond the struct
