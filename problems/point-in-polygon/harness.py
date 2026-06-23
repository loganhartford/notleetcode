from solution import point_in_polygon
from nlc import run_cases

SQUARE = [(0, 0), (4, 0), (4, 4), (0, 4)]
CONCAVE = [(0, 0), (4, 0), (4, 4), (2, 2), (0, 4)]
TRIANGLE = [(0, 0), (6, 0), (3, 5)]

CASES = [
    {"name": "square inside", "args": [SQUARE, (2, 2)], "expected": True},
    {"name": "square outside", "args": [SQUARE, (5, 2)], "expected": False},
    {"name": "concave notch (outside)", "args": [CONCAVE, (2, 3)], "expected": False},
    {"name": "concave inside", "args": [CONCAVE, (1, 1)], "expected": True},
    {"name": "triangle inside", "args": [TRIANGLE, (3, 1)], "expected": True},
    {"name": "triangle outside", "args": [TRIANGLE, (0, 4)], "expected": False},
]

run_cases(point_in_polygon, CASES, fmt_input=lambda c: "point={}".format(c["args"][1]))
