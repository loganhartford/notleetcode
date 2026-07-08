#include <queue>

class MovingAverage {
    int k_;
    std::queue<int> window_;
    long long sum_;
public:
    MovingAverage(int k) : k_(k), sum_(0) {}

    double next(int value) {
        window_.push(value);
        sum_ += value;
        if ((int)window_.size() > k_) {
            sum_ -= window_.front();
            window_.pop();
        }
        return static_cast<double>(sum_) / window_.size();
    }
};
