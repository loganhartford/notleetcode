from typing import List


def longestConsecutive(nums: List[int]) -> int:
    s = set(nums)
    best = 0
    for x in s:
        # Only start counting from the beginning of a run.
        if x - 1 not in s:
            length = 1
            while x + length in s:
                length += 1
            best = max(best, length)
    return best
