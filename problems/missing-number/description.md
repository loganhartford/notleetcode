# Missing Number

Given an array `nums` containing `n` **distinct** numbers in the range `[0, n]`, return the only number in the range that is **missing** from the array.

```c
int missingNumber(int* nums, int numsSize);
```

### Example 1
```
Input:  nums = [3, 0, 1]
Output: 2        # n = 3, range is [0,3]; 2 is missing
```

### Example 2
```
Input:  nums = [0, 1]
Output: 2
```

### Example 3
```
Input:  nums = [9,6,4,2,3,5,7,0,1]
Output: 8
```

### Constraints
- `n == len(nums)`, `1 <= n <= 10^4`
- `0 <= nums[i] <= n`, all distinct.
- XOR the indices `0..n` with all the values; everything cancels except the
  missing number. (Or use the sum formula `n(n+1)/2`.)
