#include <vector>
#include <deque>

std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
    std::deque<int> dq; // indices, monotonically decreasing values
    std::vector<int> result;
    for (int i = 0; i < (int)nums.size(); i++) {
        // Remove indices out of window
        while (!dq.empty() && dq.front() < i - k + 1) dq.pop_front();
        // Maintain decreasing order
        while (!dq.empty() && nums[dq.back()] < nums[i]) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) result.push_back(nums[dq.front()]);
    }
    return result;
}
