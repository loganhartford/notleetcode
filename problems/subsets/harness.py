from solution import subsets
from nlc import run_cases


def normalize(subs):
    return sorted([sorted(s) for s in subs])


CASES = [
    {"name": "three elements", "args": [[1, 2, 3]],
     "expected": [[], [1], [2], [3], [1, 2], [1, 3], [2, 3], [1, 2, 3]]},
    {"name": "single", "args": [[0]], "expected": [[], [0]]},
    {"name": "two", "args": [[5, 9]], "expected": [[], [5], [9], [5, 9]]},
]

run_cases(
    subsets, CASES,
    compare=lambda got, c, a: got is not None and normalize(got) == normalize(c["expected"]),
    fmt_input=lambda c: "nums={}".format(c["args"][0]),
)
