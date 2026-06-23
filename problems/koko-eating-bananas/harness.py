from solution import minEatingSpeed
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[3, 6, 7, 11], 8], "expected": 4},
    {"name": "tight (h = n)", "args": [[30, 11, 23, 4, 20], 5], "expected": 30},
    {"name": "one extra hour", "args": [[30, 11, 23, 4, 20], 6], "expected": 23},
    {"name": "single pile", "args": [[312884470], 968709470], "expected": 1},
    {"name": "lots of time", "args": [[1, 1, 1, 1], 100], "expected": 1},
]

run_cases(minEatingSpeed, CASES, fmt_input=lambda c: "piles={}, h={}".format(*c["args"]))
