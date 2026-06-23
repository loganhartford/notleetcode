from solution import dailyTemperatures
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[73, 74, 75, 71, 69, 72, 76, 73]], "expected": [1, 1, 4, 2, 1, 1, 0, 0]},
    {"name": "increasing", "args": [[30, 40, 50, 60]], "expected": [1, 1, 1, 0]},
    {"name": "short", "args": [[30, 60, 90]], "expected": [1, 1, 0]},
    {"name": "all same", "args": [[50, 50, 50]], "expected": [0, 0, 0]},
    {"name": "decreasing", "args": [[90, 80, 70]], "expected": [0, 0, 0]},
    {"name": "single", "args": [[42]], "expected": [0]},
]

run_cases(
    dailyTemperatures, CASES,
    compare=lambda got, c, a: got is not None and list(got) == c["expected"],
    fmt_input=lambda c: "temperatures={}".format(c["args"][0]),
)
