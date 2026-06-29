# Daily Temperatures

Given an array of integers `temperatures` representing daily temperatures, return an array `answer` such that `answer[i]` is the **number of days you have to wait** after day `i` to get a warmer temperature. If there is no future day for which this is possible, set `answer[i] = 0`.

### Example 1
```
Input:  temperatures = [73, 74, 75, 71, 69, 72, 76, 73]
Output: [1, 1, 4, 2, 1, 1, 0, 0]
```

### Example 2
```
Input:  temperatures = [30, 40, 50, 60]
Output: [1, 1, 1, 0]
```

### Example 3
```
Input:  temperatures = [30, 60, 90]
Output: [1, 1, 0]
```

### Constraints
- `1 <= len(temperatures) <= 10^5`
- `30 <= temperatures[i] <= 100`

**Optimal complexity:** O(n)  — monotonic stack, each element pushed/popped once
