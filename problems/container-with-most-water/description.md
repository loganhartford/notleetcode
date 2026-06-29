# Container With Most Water

You are given an integer array `height` of length `n`. There are `n` vertical lines drawn such that the two endpoints of the `i`-th line are `(i, 0)` and `(i, height[i])`.

Find two lines that, together with the x-axis, form a container that holds the **most water**. Return the maximum amount of water a container can store.

You may not slant the container — the water level is limited by the **shorter** of the two lines, and the width is the distance between them.

### Example 1
```
Input:  height = [1, 8, 6, 2, 5, 4, 8, 3, 7]
Output: 49        # lines at index 1 and 8: min(8,7) * (8-1) = 7 * 7
```

### Example 2
```
Input:  height = [1, 1]
Output: 1
```

### Constraints
- `2 <= len(height) <= 10^5`
- `0 <= height[i] <= 10^4`

**Optimal complexity:** O(n)  — two-pointer single pass
