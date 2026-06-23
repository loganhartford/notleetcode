from solution import gridDijkstra
from nlc import run_cases

CASES = [
    {"name": "around costly center", "args": [[[1, 1, 1], [1, 9, 1], [1, 1, 1]], [0, 0], [2, 2]], "expected": 4},
    {"name": "around a wall", "args": [[[1, -1, 1], [1, -1, 1], [1, 1, 1]], [0, 0], [0, 2]], "expected": 6},
    {"name": "blocked", "args": [[[0, -1], [-1, 0]], [0, 0], [1, 1]], "expected": -1},
    {"name": "start == goal", "args": [[[5, 5], [5, 5]], [0, 0], [0, 0]], "expected": 0},
    {"name": "straight line", "args": [[[0, 2, 3, 4]], [0, 0], [0, 3]], "expected": 9},
    {"name": "goal is obstacle", "args": [[[1, 1], [1, -1]], [0, 0], [1, 1]], "expected": -1},
]

run_cases(gridDijkstra, CASES, fmt_input=lambda c: "start={}, goal={}".format(c["args"][1], c["args"][2]))
