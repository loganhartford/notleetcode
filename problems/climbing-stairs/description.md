# Climbing Stairs

You are climbing a staircase. It takes `n` steps to reach the top. Each time you
can climb either **1** or **2** steps. In how many **distinct ways** can you climb
to the top?

### Example 1
```
Input:  n = 2
Output: 2        # (1+1), (2)
```

### Example 2
```
Input:  n = 3
Output: 3        # (1+1+1), (1+2), (2+1)
```

### Example 3
```
Input:  n = 5
Output: 8
```

### Constraints
- `1 <= n <= 45`
- This is the DP intro: `ways(n) = ways(n-1) + ways(n-2)` — it's Fibonacci.
