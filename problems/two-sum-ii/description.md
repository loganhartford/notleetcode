# Two Sum II (Input Array Is Sorted)

Given a **1-indexed** array of integers `numbers` that is already sorted in **non-decreasing order**, find two numbers that add up to a specific `target`.

Return the indices of the two numbers, `index1` and `index2`, **1-indexed**, as `[index1, index2]` with `index1 < index2`.

There is exactly one solution. You may **not** use the same element twice, and you must use **only constant extra space**.

### Example 1
```
Input:  numbers = [2, 7, 11, 15], target = 9
Output: [1, 2]        # 2 + 7 == 9  → 1-indexed positions 1 and 2
```

### Example 2
```
Input:  numbers = [2, 3, 4], target = 6
Output: [1, 3]
```

### Example 3
```
Input:  numbers = [-1, 0], target = -1
Output: [1, 2]
```

### Constraints
- `2 <= len(numbers) <= 3 * 10^4`
- `numbers` is sorted non-decreasing.
- Exactly one solution exists.
