from solution import numIslands
from nlc import run_cases

CASES = [
    {"name": "one island", "args": [[
        ["1", "1", "1", "1", "0"],
        ["1", "1", "0", "1", "0"],
        ["1", "1", "0", "0", "0"],
        ["0", "0", "0", "0", "0"]]], "expected": 1},
    {"name": "three islands", "args": [[
        ["1", "1", "0", "0", "0"],
        ["1", "1", "0", "0", "0"],
        ["0", "0", "1", "0", "0"],
        ["0", "0", "0", "1", "1"]]], "expected": 3},
    {"name": "all water", "args": [[["0", "0"], ["0", "0"]]], "expected": 0},
    {"name": "all land", "args": [[["1", "1"], ["1", "1"]]], "expected": 1},
    {"name": "diagonal not connected", "args": [[["1", "0"], ["0", "1"]]], "expected": 2},
]

run_cases(numIslands, CASES, fmt_input=lambda c: "grid (rows={})".format(len(c["args"][0])))
