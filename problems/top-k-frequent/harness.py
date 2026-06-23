from solution import topKFrequent
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[1, 1, 1, 2, 2, 3], 2], "expected": [1, 2]},
    {"name": "single", "args": [[1], 1], "expected": [1]},
    {"name": "k = distinct", "args": [[4, 4, 5, 5, 6], 3], "expected": [4, 5, 6]},
    {"name": "negatives", "args": [[-1, -1, -1, 2, 2, 3], 1], "expected": [-1]},
    {"name": "top two", "args": [[5, 5, 5, 3, 3, 1, 1, 1, 1], 2], "expected": [1, 5]},
]

run_cases(
    topKFrequent, CASES,
    compare=lambda got, c, a: got is not None and sorted(got) == sorted(c["expected"]),
    fmt_input=lambda c: "nums={}, k={}".format(c["args"][0], c["args"][1]),
)
