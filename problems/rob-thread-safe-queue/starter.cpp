#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(size_t capacity) {
        // TODO: store capacity
    }

    void push(T value) {
        // TODO: block until space available, then push
    }

    T pop() {
        // TODO: block until item available, then return it
        return T{};
    }

    bool try_push(T value) {
        // TODO: push if space available, return false if full
        return false;
    }

    std::optional<T> try_pop() {
        // TODO: pop if item available, return nullopt if empty
        return std::nullopt;
    }

    size_t size() const {
        // TODO: return current element count
        return 0;
    }
};
