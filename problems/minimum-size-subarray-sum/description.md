# Minimum Size Subarray Sum

Given an array of **positive** integers `nums` and a positive integer `target`, return the **minimal length** of a contiguous subarray of which the sum is **greater than or equal to** `target`. If there is no such subarray, return `0`.

### Example 1
```
Input:  target = 7, nums = [2, 3, 1, 2, 4, 3]
Output: 2        # [4, 3] has the minimal length
```

### Example 2
```
Input:  target = 4, nums = [1, 4, 4]
Output: 1
```

### Example 3
```
Input:  target = 11, nums = [1, 1, 1, 1, 1, 1, 1, 1]
Output: 0
```

### Constraints
- `1 <= target <= 10^9`
- `1 <= len(nums) <= 10^5`
- `1 <= nums[i] <= 10^4`

> **Note:** signature is `minSubArrayLen(target, nums)` — target first.

**Optimal complexity:** O(n)  — sliding window, each element enters and leaves at most once
