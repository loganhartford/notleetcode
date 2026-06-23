from typing import List


def maxProfit(prices: List[int]) -> int:
    best = 0
    lo = prices[0]
    for p in prices[1:]:
        lo = min(lo, p)
        best = max(best, p - lo)
    return best
