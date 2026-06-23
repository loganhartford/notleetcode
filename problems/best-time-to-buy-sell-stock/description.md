# Best Time to Buy and Sell Stock

You are given an array `prices` where `prices[i]` is the price of a given stock on day `i`.

You want to maximize your profit by choosing a **single day to buy** one stock and a **different day in the future to sell** it. Return the maximum profit you can achieve. If no profit is possible, return `0`.

### Example 1
```
Input:  prices = [7, 1, 5, 3, 6, 4]
Output: 5        # buy at 1 (day 1), sell at 6 (day 4)
```

### Example 2
```
Input:  prices = [7, 6, 4, 3, 1]
Output: 0        # prices only fall — no profitable trade
```

### Constraints
- `1 <= len(prices) <= 10^5`
- `0 <= prices[i] <= 10^4`
