from collections import deque


class MovingAverage:
    def __init__(self, window: int):
        self.buf = deque(maxlen=window)

    def next(self, val: float) -> float:
        self.buf.append(val)
        return sum(self.buf) / len(self.buf)


class MedianFilter:
    def __init__(self, window: int):
        self.buf = deque(maxlen=window)

    def next(self, val: float) -> float:
        self.buf.append(val)
        s = sorted(self.buf)
        n = len(s)
        mid = n // 2
        if n % 2 == 1:
            return float(s[mid])
        return (s[mid - 1] + s[mid]) / 2
