from solution import networkDelayTime
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[[2, 1, 1], [2, 3, 1], [3, 4, 1]], 4, 2], "expected": 2},
    {"name": "single edge", "args": [[[1, 2, 1]], 2, 1], "expected": 1},
    {"name": "unreachable", "args": [[[1, 2, 1]], 2, 2], "expected": -1},
    {"name": "self only", "args": [[], 1, 1], "expected": 0},
    {"name": "branching", "args": [[[1, 2, 1], [1, 3, 4], [2, 3, 1]], 3, 1], "expected": 2},
]

run_cases(networkDelayTime, CASES, fmt_input=lambda c: "times={}, n={}, k={}".format(*c["args"]))
