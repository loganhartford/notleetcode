# House Robber

You are a robber planning to rob houses along a street. Each house has a certain
amount of money stashed (`nums[i]`). The only constraint: **adjacent houses have
connected security systems**, so you cannot rob two adjacent houses on the same
night.

Return the **maximum amount** of money you can rob tonight without alerting the
police.

### Example 1
```
Input:  nums = [1, 2, 3, 1]
Output: 4        # rob house 0 (1) and house 2 (3)
```

### Example 2
```
Input:  nums = [2, 7, 9, 3, 1]
Output: 12       # rob houses 0, 2, 4 -> 2 + 9 + 1
```

### Example 3
```
Input:  nums = [5]
Output: 5
```

### Constraints
- `1 <= len(nums) <= 100`
- `0 <= nums[i] <= 400`
- DP: `best(i) = max(best(i-1), best(i-2) + nums[i])`.
