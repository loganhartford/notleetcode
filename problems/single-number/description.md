# Single Number

Given a non-empty array of integers `nums`, every element appears **twice** except for one. Find that single one.

You must implement a solution with **linear runtime complexity** and use only **constant extra space**.

```c
int singleNumber(int* nums, int numsSize);
```

### Example 1
```
Input:  nums = [2, 2, 1]
Output: 1
```

### Example 2
```
Input:  nums = [4, 1, 2, 1, 2]
Output: 4
```

### Example 3
```
Input:  nums = [1]
Output: 1
```

### Constraints
- `1 <= len(nums) <= 3 * 10^4`
- `-3 * 10^4 <= nums[i] <= 3 * 10^4`
- XOR is its own inverse: `a ^ a == 0`, so XOR-ing everything cancels the pairs.

**Optimal complexity:** O(n)  — XOR cancellation
