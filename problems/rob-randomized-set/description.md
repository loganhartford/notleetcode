# Randomized Set O(1)

Design a set that supports insert, remove, and uniform random selection, all in **O(1)** average time.

```cpp
class RandomizedSet {
public:
    bool insert(int val);   // insert val; returns false if already present
    bool remove(int val);   // remove val; returns false if not present
    int getRandom();        // return a uniform random element; set is non-empty
};
```

## Example

```
RandomizedSet rs;
rs.insert(1)  → true
rs.insert(2)  → true
rs.insert(1)  → false  (already present)
rs.remove(1)  → true
rs.getRandom() → 2     (only element)
```

## Constraints

- `-2^31 <= val <= 2^31 - 1`
- At most 2 * 10^5 calls
- `getRandom()` is only called when set is non-empty

**Optimal complexity:** O(1) per operation — store values in a vector; use a hash map from value to its index for O(1) delete (swap with last, then pop).
