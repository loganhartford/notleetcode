import math
from solution import pure_pursuit
from nlc import run_cases


def drive(pose, path, lookahead):
    return round(pure_pursuit(pose, path, lookahead), 6)


def close(got, case, args):
    return got is not None and abs(got - case["expected"]) < 1e-6


CASES = [
    {"name": "straight ahead", "args": [(0, 0, 0), [(1, 0), (2, 0)], 1.0], "expected": 0.0},
    {"name": "target up-left", "args": [(0, 0, 0), [(2, 2)], 2.0], "expected": 1.0},
    {"name": "heading rotated", "args": [(0, 0, math.pi / 2), [(1, 0), (3, 0)], 2.0], "expected": -1.5},
    {"name": "target behind-right", "args": [(0, 0, 0), [(0, -2)], 2.0], "expected": -1.0},
]

run_cases(drive, CASES, compare=close, fmt_input=lambda c: "pose={}, path={}, L={}".format(*c["args"]))
