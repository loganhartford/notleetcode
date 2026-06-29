# Subsets

Given an integer array `nums` of **unique** elements, return **all possible
subsets** (the power set).

The solution set must not contain duplicate subsets. Return the subsets in any
order; the checker is order-independent (both the list of subsets and the order
within each subset).

### Example 1
```
Input:  nums = [1, 2, 3]
Output: [[], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]]
```

### Example 2
```
Input:  nums = [0]
Output: [[], [0]]
```

### Constraints
- `1 <= len(nums) <= 10`
- `-10 <= nums[i] <= 10`
- All elements are unique.
- This is the **backtracking template**: at each index, choose to include the
  element or not.

**Optimal complexity:** O(n · 2ⁿ)  — enumerate all 2ⁿ subsets
