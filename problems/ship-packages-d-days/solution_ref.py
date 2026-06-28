# Time: O(n · log(sum(weights)))  — binary search on capacity, O(n) feasibility check
from typing import List


def shipWithinDays(weights: List[int], days: int) -> int:
    def days_needed(cap: int) -> int:
        d, load = 1, 0
        for w in weights:
            if load + w > cap:
                d += 1
                load = 0
            load += w
        return d

    lo, hi = max(weights), sum(weights)
    while lo < hi:
        mid = (lo + hi) // 2
        if days_needed(mid) <= days:
            hi = mid
        else:
            lo = mid + 1
    return lo
