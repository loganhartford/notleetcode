# Time: O(n)  — sliding window, each element enters and leaves at most once
from typing import List


def minSubArrayLen(target: int, nums: List[int]) -> int:
    left = 0
    total = 0
    best = float("inf")
    for right, x in enumerate(nums):
        total += x
        while total >= target:
            best = min(best, right - left + 1)
            total -= nums[left]
            left += 1
    return 0 if best == float("inf") else best
