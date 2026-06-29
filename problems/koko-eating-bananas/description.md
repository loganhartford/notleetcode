# Koko Eating Bananas

Koko loves bananas. There are `n` piles, where the `i`-th pile has `piles[i]` bananas. The guards have gone and will come back in `h` hours.

Koko decides her constant eating speed `k` (bananas per hour). Each hour she picks a pile and eats `k` bananas from it; if the pile has fewer than `k`, she eats them all and does not eat more that hour.

Return the **minimum integer speed `k`** such that she can eat all the bananas within `h` hours.

### Example 1
```
Input:  piles = [3, 6, 7, 11], h = 8
Output: 4
```

### Example 2
```
Input:  piles = [30, 11, 23, 4, 20], h = 5
Output: 30
```

### Example 3
```
Input:  piles = [30, 11, 23, 4, 20], h = 6
Output: 23
```

### Constraints
- `1 <= len(piles) <= 10^4`
- `1 <= piles[i] <= 10^9`
- `len(piles) <= h <= 10^9`

**Optimal complexity:** O(n · log(max(piles)))  — binary search on speed, O(n) feasibility check
