import math
from solution import transform_point, compose, invert
from nlc import run_cases


def drive(kind, a, b):
    if kind == "tp":
        return tuple(round(v, 6) for v in transform_point(a, b))
    if kind == "compose":
        return tuple(round(v, 6) for v in compose(a, b))
    if kind == "invert":
        return tuple(round(v, 6) for v in invert(a))
    raise ValueError(kind)


def close(got, case, args):
    exp = case["expected"]
    if got is None or len(got) != len(exp):
        return False
    return all(abs(g - e) < 1e-6 for g, e in zip(got, exp))


HALF_PI = round(math.pi / 2, 6)

CASES = [
    {"name": "translate point", "args": ["tp", (1, 2, 0), (3, 4)], "expected": (4.0, 6.0)},
    {"name": "rotate point 90deg", "args": ["tp", (0, 0, math.pi / 2), (1, 0)], "expected": (0.0, 1.0)},
    {"name": "rotate 180 + translate", "args": ["tp", (1, 1, math.pi), (2, 0)], "expected": (-1.0, 1.0)},
    {"name": "compose", "args": ["compose", (1, 0, math.pi / 2), (1, 0, 0)], "expected": (1.0, 1.0, HALF_PI)},
    {"name": "invert", "args": ["invert", (1, 2, math.pi / 2), None], "expected": (-2.0, 1.0, -HALF_PI)},
]

run_cases(drive, CASES, compare=close, fmt_input=lambda c: "{} {}".format(c["args"][0], c["args"][1:]))
