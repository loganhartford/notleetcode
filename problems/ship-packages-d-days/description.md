# Capacity To Ship Packages Within D Days

A conveyor belt has packages that must be shipped from one port to another within `days` days.

The `i`-th package has weight `weights[i]`. Each day, the ship loads packages **in the given order**. The total weight loaded in a day cannot exceed the ship's capacity.

Return the **least weight capacity** of the ship that will let all the packages be shipped within `days` days.

### Example 1
```
Input:  weights = [1,2,3,4,5,6,7,8,9,10], days = 5
Output: 15
```

### Example 2
```
Input:  weights = [3,2,2,4,1,4], days = 3
Output: 6
```

### Example 3
```
Input:  weights = [1,2,3,1,1], days = 4
Output: 3
```

### Constraints
- `1 <= days <= len(weights) <= 5 * 10^4`
- `1 <= weights[i] <= 500`

Packages must ship in order; the capacity is at least `max(weights)` and at most `sum(weights)`.
