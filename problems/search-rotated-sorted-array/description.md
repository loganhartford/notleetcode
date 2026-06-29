# Search in Rotated Sorted Array

There is an integer array `nums` sorted in ascending order (with **distinct** values), which has been **rotated** at an unknown pivot. For example, `[0,1,2,4,5,6,7]` might become `[4,5,6,7,0,1,2]`.

Given the rotated array `nums` and an integer `target`, return the **index** of `target` if it is in `nums`, or `-1` if it is not.

You must write an algorithm with **O(log n)** runtime complexity.

### Example 1
```
Input:  nums = [4, 5, 6, 7, 0, 1, 2], target = 0
Output: 4
```

### Example 2
```
Input:  nums = [4, 5, 6, 7, 0, 1, 2], target = 3
Output: -1
```

### Example 3
```
Input:  nums = [1], target = 0
Output: -1
```

### Constraints
- `1 <= len(nums) <= 5000`
- `-10^4 <= nums[i], target <= 10^4`
- All values of `nums` are unique; `nums` is a rotated ascending array.

**Optimal complexity:** O(log n)  — binary search on rotated array
