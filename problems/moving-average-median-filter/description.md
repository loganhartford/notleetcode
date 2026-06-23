# Moving-Average Filter, then Median Filter

Implement two streaming filters with a fixed window size. Both process values one
at a time and return the filtered value over the **most recent `window` samples**
(or fewer, while the window is still filling).

```python
class MovingAverage:
    def __init__(self, window: int): ...
    def next(self, val: float) -> float:   # mean of the last `window` samples

class MedianFilter:
    def __init__(self, window: int): ...
    def next(self, val: float) -> float:   # median of the last `window` samples
```

- **MovingAverage.next** returns the arithmetic mean of the samples currently in
  the window.
- **MedianFilter.next** returns the median. For an **even** number of samples,
  return the **average of the two middle** values. The median filter is the one
  that rejects single-sample spikes — that's why robotics code reaches for it.

### Example
```
MovingAverage(3): next(1)=1.0, next(2)=1.5, next(3)=2.0, next(7)=4.0   # (2+3+7)/3
MedianFilter(3):  next(1)=1.0, next(2)=1.5, next(3)=2.0, next(7)=3.0   # median(2,3,7)
```

### Notes
- While the window isn't full yet, filter over however many samples you have.
- `[author]`-defined robotics practice; small float tolerance allowed.
