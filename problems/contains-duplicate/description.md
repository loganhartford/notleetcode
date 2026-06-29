# Contains Duplicate

Given an integer array `nums`, return `True` if any value appears **at least twice** in the array, and `False` if every element is distinct.

### Example 1
```
Input:  nums = [1, 2, 3, 1]
Output: True
```

### Example 2
```
Input:  nums = [1, 2, 3, 4]
Output: False
```

### Example 3
```
Input:  nums = [1, 1, 1, 3, 3, 4, 3, 2, 4, 2]
Output: True
```

### Constraints
- `1 <= len(nums) <= 10^5`
- `-10^9 <= nums[i] <= 10^9`

**Optimal complexity:** O(n)  — hash set
