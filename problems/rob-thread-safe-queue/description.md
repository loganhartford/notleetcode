# Thread-Safe Bounded Queue

Design a thread-safe queue with a fixed capacity. Blocking operations wait when the queue is full or empty; non-blocking operations return immediately.

```cpp
template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(size_t capacity);
    void push(T value);              // blocks until space is available
    T pop();                         // blocks until an item is available
    bool try_push(T value);          // returns false immediately if full
    std::optional<T> try_pop();      // returns nullopt immediately if empty
    size_t size() const;             // current number of elements
};
```

Use `std::mutex` and `std::condition_variable` for synchronization.

## Constraints

- `capacity >= 1`
- `push` and `pop` must be safe to call from multiple threads simultaneously
- `size()` is an approximate snapshot (no strong ordering guarantee)

**Optimal complexity:** O(1) per operation — circular buffer or `std::queue` protected by a mutex.
