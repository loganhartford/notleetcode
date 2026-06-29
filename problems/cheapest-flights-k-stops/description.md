# Cheapest Flights Within K Stops

There are `n` cities connected by some number of flights. You are given `flights` where `flights[i] = [from, to, price]`.

You are also given three integers `src`, `dst`, and `k`. Return the **cheapest price** from `src` to `dst` with **at most `k` stops**. If there is no such route, return `-1`.

"At most `k` stops" means the path uses at most `k + 1` flights.

### Example 1
```
Input:  n = 4, flights = [[0,1,100],[1,2,100],[2,0,100],[1,3,600],[2,3,200]],
        src = 0, dst = 3, k = 1
Output: 700      # 0 -> 1 -> 3
```

### Example 2
```
Input:  n = 3, flights = [[0,1,100],[1,2,100],[0,2,500]], src = 0, dst = 2, k = 1
Output: 200      # 0 -> 1 -> 2
```

### Example 3
```
Input:  n = 3, flights = [[0,1,100],[1,2,100],[0,2,500]], src = 0, dst = 2, k = 0
Output: 500      # only the direct flight is allowed
```

### Constraints
- `1 <= n <= 100`
- `0 <= len(flights) <= n * (n - 1) / 2`
- `0 <= src, dst, k < n`, `src != dst`
- `1 <= price <= 10^4`

Bellman-Ford for exactly `k+1` relaxations is the clean approach.

**Optimal complexity:** O(k · E)  — Bellman-Ford limited to k+1 relaxation rounds
