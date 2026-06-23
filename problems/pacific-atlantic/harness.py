from solution import pacificAtlantic
from nlc import run_cases

CASES = [
    {"name": "example", "args": [[
        [1, 2, 2, 3, 5],
        [3, 2, 3, 4, 4],
        [2, 4, 5, 3, 1],
        [6, 7, 1, 4, 5],
        [5, 1, 1, 2, 4]]],
     "expected": [[0, 4], [1, 3], [1, 4], [2, 2], [3, 0], [3, 1], [4, 0]]},
    {"name": "single cell", "args": [[[1]]], "expected": [[0, 0]]},
    {"name": "single row", "args": [[[1, 2, 3]]], "expected": [[0, 0], [0, 1], [0, 2]]},
    {"name": "flat 2x2", "args": [[[1, 1], [1, 1]]], "expected": [[0, 0], [0, 1], [1, 0], [1, 1]]},
]


def normalize(coords):
    return sorted([list(c) for c in coords])


run_cases(
    pacificAtlantic, CASES,
    compare=lambda got, c, a: got is not None and normalize(got) == normalize(c["expected"]),
    fmt_input=lambda c: "heights (rows={})".format(len(c["args"][0])),
)
