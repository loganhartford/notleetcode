# Permutations

Given an array `nums` of **distinct** integers, return **all the possible
permutations**. You may return the answer in any order; the checker is
order-independent (the list of permutations may be in any order).

### Example 1
```
Input:  nums = [1, 2, 3]
Output: [[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]
```

### Example 2
```
Input:  nums = [0, 1]
Output: [[0,1],[1,0]]
```

### Example 3
```
Input:  nums = [1]
Output: [[1]]
```

### Constraints
- `1 <= len(nums) <= 6`
- `-10 <= nums[i] <= 10`
- All integers are distinct.
- Backtracking: pick an unused element for each position. The order **within**
  each permutation matters (do not sort it).
