# Time: O(n)  — frequency count then bucket sort by frequency
from typing import List
from collections import Counter


def topKFrequent(nums: List[int], k: int) -> List[int]:
    counts = Counter(nums)
    # Bucket by frequency, then read from the highest bucket down.
    buckets = [[] for _ in range(len(nums) + 1)]
    for val, freq in counts.items():
        buckets[freq].append(val)
    res = []
    for freq in range(len(nums), 0, -1):
        for val in buckets[freq]:
            res.append(val)
            if len(res) == k:
                return res
    return res
