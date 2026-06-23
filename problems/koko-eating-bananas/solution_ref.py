from typing import List
import math


def minEatingSpeed(piles: List[int], h: int) -> int:
    def hours_at(k: int) -> int:
        return sum(math.ceil(p / k) for p in piles)

    lo, hi = 1, max(piles)
    while lo < hi:
        mid = (lo + hi) // 2
        if hours_at(mid) <= h:
            hi = mid
        else:
            lo = mid + 1
    return lo
