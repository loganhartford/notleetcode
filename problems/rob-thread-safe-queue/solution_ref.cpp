#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template <typename T>
class BoundedQueue {
    size_t cap_;
    std::queue<T> q_;
    mutable std::mutex mtx_;
    std::condition_variable not_full_, not_empty_;
public:
    explicit BoundedQueue(size_t capacity) : cap_(capacity) {}

    void push(T value) {
        std::unique_lock<std::mutex> lock(mtx_);
        not_full_.wait(lock, [&]{ return q_.size() < cap_; });
        q_.push(std::move(value));
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        not_empty_.wait(lock, [&]{ return !q_.empty(); });
        T val = std::move(q_.front());
        q_.pop();
        not_full_.notify_one();
        return val;
    }

    bool try_push(T value) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (q_.size() >= cap_) return false;
        q_.push(std::move(value));
        not_empty_.notify_one();
        return true;
    }

    std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lock(mtx_);
        if (q_.empty()) return std::nullopt;
        T val = std::move(q_.front());
        q_.pop();
        not_full_.notify_one();
        return val;
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return q_.size();
    }
};
