#include <vector>
#include <unordered_map>
#include <cstdlib>

class RandomizedSet {
    std::vector<int> vals_;
    std::unordered_map<int, int> idx_; // val → index in vals_
public:
    bool insert(int val) {
        if (idx_.count(val)) return false;
        idx_[val] = vals_.size();
        vals_.push_back(val);
        return true;
    }

    bool remove(int val) {
        auto it = idx_.find(val);
        if (it == idx_.end()) return false;
        int pos = it->second;
        int last = vals_.back();
        vals_[pos] = last;
        idx_[last] = pos;
        vals_.pop_back();
        idx_.erase(val);
        return true;
    }

    int getRandom() {
        return vals_[rand() % vals_.size()];
    }
};
