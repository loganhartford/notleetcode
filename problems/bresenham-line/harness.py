from solution import bresenham, line_of_sight
from nlc import run_cases


def drive(kind, a, b):
    if kind == "line":
        return [tuple(p) for p in bresenham(*a)]
    if kind == "los":
        return line_of_sight(a, b[0], b[1])
    raise ValueError(kind)


GRID = [[0, 0, 0], [0, 1, 0], [0, 0, 0]]

CASES = [
    {"name": "shallow line", "args": ["line", (0, 0, 3, 1), None], "expected": [(0, 0), (1, 0), (2, 1), (3, 1)]},
    {"name": "vertical down", "args": ["line", (2, 2, 2, 0), None], "expected": [(2, 2), (2, 1), (2, 0)]},
    {"name": "diagonal", "args": ["line", (0, 0, 2, 2), None], "expected": [(0, 0), (1, 1), (2, 2)]},
    {"name": "reverse octant", "args": ["line", (5, 5, 0, 2), None],
     "expected": [(5, 5), (4, 4), (3, 4), (2, 3), (1, 3), (0, 2)]},
    {"name": "los blocked", "args": ["los", GRID, [(0, 0), (2, 2)]], "expected": False},
    {"name": "los clear edge", "args": ["los", GRID, [(0, 0), (2, 0)]], "expected": True},
]

run_cases(drive, CASES, fmt_input=lambda c: "{} {}".format(c["args"][0], c["args"][1]))
