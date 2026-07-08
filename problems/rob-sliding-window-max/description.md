# Sliding Window Maximum

Given an array of integers and a window size `k`, return a list containing the maximum value of each sliding window.

```cpp
vector<int> maxSlidingWindow(vector<int>& nums, int k);
```

The window slides one position to the right each step. The output has `n - k + 1` elements.

## Example

```
nums = [1,3,-1,-3,5,3,6,7], k = 3
Windows:     Max
[1,3,-1]  →  3
[3,-1,-3] →  3
[-1,-3,5] →  5
[-3,5,3]  →  5
[5,3,6]   →  6
[3,6,7]   →  7

Output: [3,3,5,5,6,7]
```

## Constraints

- `1 <= nums.length <= 10^5`
- `-10^4 <= nums[i] <= 10^4`
- `1 <= k <= nums.length`

**Optimal complexity:** O(n) — maintain a monotonically decreasing deque of indices; the front is always the current window maximum.
