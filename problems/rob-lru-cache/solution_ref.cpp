#include <unordered_map>
#include <list>

class LRUCache {
    int cap_;
    std::list<std::pair<int,int>> lst_; // {key, value}, front=MRU
    std::unordered_map<int, std::list<std::pair<int,int>>::iterator> map_;
public:
    LRUCache(int capacity) : cap_(capacity) {}

    int get(int key) {
        auto it = map_.find(key);
        if (it == map_.end()) return -1;
        lst_.splice(lst_.begin(), lst_, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second->second = value;
            lst_.splice(lst_.begin(), lst_, it->second);
            return;
        }
        if ((int)lst_.size() == cap_) {
            auto& lru = lst_.back();
            map_.erase(lru.first);
            lst_.pop_back();
        }
        lst_.push_front({key, value});
        map_[key] = lst_.begin();
    }
};
