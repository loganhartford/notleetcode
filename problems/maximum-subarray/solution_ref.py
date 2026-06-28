# Time: O(n)  — Kadane's algorithm
from typing import List


def maxSubArray(nums: List[int]) -> int:
    best = cur = nums[0]
    for x in nums[1:]:
        cur = max(x, cur + x)
        best = max(best, cur)
    return best
