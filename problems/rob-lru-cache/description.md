# LRU Cache

Design a data structure that follows the **Least Recently Used (LRU)** cache eviction policy.

```cpp
class LRUCache {
public:
    LRUCache(int capacity);
    int get(int key);              // returns value or -1 if not found; updates recency
    void put(int key, int value);  // inserts or updates; evicts LRU if at capacity
};
```

Both `get` and `put` must run in **O(1)** average time.

## Example

```
LRUCache cache(2);
cache.put(1, 1);   // cache: {1:1}
cache.put(2, 2);   // cache: {1:1, 2:2}
cache.get(1)  → 1  // makes 1 most recent; cache: {2:2, 1:1}
cache.put(3, 3);   // evicts key 2 (LRU); cache: {1:1, 3:3}
cache.get(2)  → -1 // key 2 was evicted
```

## Constraints

- `1 <= capacity <= 3000`
- `0 <= key, value <= 10^4`
- At most 10^5 calls

**Optimal complexity:** O(1) per operation — combine a hash map with a doubly-linked list.
