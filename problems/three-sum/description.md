# 3Sum

Given an integer array `nums`, return all the triplets `[nums[i], nums[j], nums[k]]` such that `i`, `j`, and `k` are distinct indices and `nums[i] + nums[j] + nums[k] == 0`.

The solution set **must not contain duplicate triplets**. The order of triplets and the order within a triplet does not matter (the checker normalizes both).

### Example 1
```
Input:  nums = [-1, 0, 1, 2, -1, -4]
Output: [[-1, -1, 2], [-1, 0, 1]]
```

### Example 2
```
Input:  nums = [0, 1, 1]
Output: []        # no triplet sums to 0
```

### Example 3
```
Input:  nums = [0, 0, 0]
Output: [[0, 0, 0]]
```

### Constraints
- `3 <= len(nums) <= 3000`
- `-10^5 <= nums[i] <= 10^5`
