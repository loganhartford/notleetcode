from typing import List

MEDIAN_MAX = 15


class Median:
    def __init__(self) -> None:
        self.ring = [0] * MEDIAN_MAX
        self.n = 0
        self.head = 0
        self.count = 0


def _select_kth(a: List[int], k: int) -> int:
    lo, hi = 0, len(a) - 1
    while lo < hi:
        pivot = a[k]
        i, j = lo, hi
        while i <= j:
            while a[i] < pivot:
                i += 1
            while pivot < a[j]:
                j -= 1
            if i <= j:
                a[i], a[j] = a[j], a[i]
                i += 1
                j -= 1
        if j < k:
            lo = i
        if k < i:
            hi = j
    return a[k]


def median_init(m: Median, n: int) -> None:
    m.n = n
    m.head = 0
    m.count = 0


def median_update(m: Median, sample: int) -> int:
    m.ring[m.head] = sample
    m.head = (m.head + 1) % m.n
    if m.count < m.n:
        m.count += 1
    return _select_kth(m.ring[:m.count], (m.count - 1) // 2)
