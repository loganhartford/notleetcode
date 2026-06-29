# Top K Frequent Elements

Given an integer array `nums` and an integer `k`, return the `k` **most frequent** elements. You may return the answer in any order.

It is guaranteed that the answer is **unique** (the set of `k` most frequent elements is well-defined).

### Example 1
```
Input:  nums = [1, 1, 1, 2, 2, 3], k = 2
Output: [1, 2]
```

### Example 2
```
Input:  nums = [1], k = 1
Output: [1]
```

### Constraints
- `1 <= len(nums) <= 10^5`
- `k` is in the range `[1, number of distinct elements]`.
- The answer is unique.

The checker is order-independent.

**Optimal complexity:** O(n)  — frequency count then bucket sort by frequency
