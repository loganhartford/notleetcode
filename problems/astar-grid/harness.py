from solution import astar
from nlc import run_cases

CASES = [
    {"name": "around barrier", "args": [[[0, 0, 0], [1, 1, 0], [0, 0, 0]], [0, 0], [2, 0]], "expected": 6},
    {"name": "open 2x2", "args": [[[0, 0], [0, 0]], [0, 0], [1, 1]], "expected": 2},
    {"name": "sealed corner", "args": [[[0, 1], [1, 0]], [0, 0], [1, 1]], "expected": -1},
    {"name": "start == goal", "args": [[[0, 0], [0, 0]], [1, 1], [1, 1]], "expected": 0},
    {"name": "straight corridor", "args": [[[0, 0, 0, 0, 0]], [0, 0], [0, 4]], "expected": 4},
    {"name": "detour", "args": [[[0, 0, 0, 0], [0, 1, 1, 0], [0, 0, 0, 0]], [0, 0], [0, 3]], "expected": 3},
]

run_cases(astar, CASES, fmt_input=lambda c: "start={}, goal={}".format(c["args"][1], c["args"][2]))
