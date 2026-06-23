from solution import MovingAverage, MedianFilter
from nlc import run_cases


def drive(kind, window, vals):
    f = MovingAverage(window) if kind == "avg" else MedianFilter(window)
    return [round(f.next(v), 6) for v in vals]


def close(got, case, args):
    exp = case["expected"]
    if got is None or len(got) != len(exp):
        return False
    return all(abs(g - e) < 1e-6 for g, e in zip(got, exp))


CASES = [
    {"name": "moving average w=3", "args": ["avg", 3, [1, 2, 3, 7, 7, 7]],
     "expected": [1.0, 1.5, 2.0, 4.0, 5.666667, 7.0]},
    {"name": "median w=3", "args": ["med", 3, [1, 2, 3, 7, 7, 7]],
     "expected": [1.0, 1.5, 2.0, 3.0, 7.0, 7.0]},
    {"name": "moving average w=1 (passthrough)", "args": ["avg", 1, [5, 9, 2]],
     "expected": [5.0, 9.0, 2.0]},
    {"name": "median rejects spike", "args": ["med", 4, [10, 10, 100, 10, 10]],
     "expected": [10.0, 10.0, 10.0, 10.0, 10.0]},
]

run_cases(drive, CASES, compare=close, fmt_input=lambda c: "{} window={}, vals={}".format(*c["args"]))
