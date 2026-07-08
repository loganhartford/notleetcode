#include <vector>
#include <algorithm>

std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>> intervals) {
    if (intervals.empty()) return {};
    std::sort(intervals.begin(), intervals.end());
    std::vector<std::vector<int>> result;
    result.push_back(intervals[0]);
    for (int i = 1; i < (int)intervals.size(); i++) {
        auto& back = result.back();
        if (intervals[i][0] <= back[1]) {
            back[1] = std::max(back[1], intervals[i][1]);
        } else {
            result.push_back(intervals[i]);
        }
    }
    return result;
}
