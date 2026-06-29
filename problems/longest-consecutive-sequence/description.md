# Longest Consecutive Sequence

Given an unsorted array of integers `nums`, return the length of the **longest consecutive elements sequence**.

A consecutive sequence is a run of integers each one greater than the last (e.g. `3, 4, 5, 6`). The elements may appear in any order in `nums`.

You must write an algorithm that runs in **O(n)** time.

### Example 1
```
Input:  nums = [100, 4, 200, 1, 3, 2]
Output: 4        # the sequence [1, 2, 3, 4]
```

### Example 2
```
Input:  nums = [0, 3, 7, 2, 5, 8, 4, 6, 0, 1]
Output: 9        # [0..8]
```

### Example 3
```
Input:  nums = []
Output: 0
```

### Constraints
- `0 <= len(nums) <= 10^5`
- `-10^9 <= nums[i] <= 10^9`

**Optimal complexity:** O(n)  — hash set; each run traversed exactly once
