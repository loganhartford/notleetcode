from solution import permute
from nlc import run_cases


def normalize(perms):
    # Order of the permutations doesn't matter, but order within each does.
    return sorted([list(p) for p in perms])


CASES = [
    {"name": "three", "args": [[1, 2, 3]],
     "expected": [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]},
    {"name": "two", "args": [[0, 1]], "expected": [[0, 1], [1, 0]]},
    {"name": "single", "args": [[7]], "expected": [[7]]},
]

run_cases(
    permute, CASES,
    compare=lambda got, c, a: got is not None and normalize(got) == normalize(c["expected"]),
    fmt_input=lambda c: "nums={}".format(c["args"][0]),
)
