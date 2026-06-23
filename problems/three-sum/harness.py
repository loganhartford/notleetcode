from solution import threeSum
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [[-1, 0, 1, 2, -1, -4]], "expected": [[-1, -1, 2], [-1, 0, 1]]},
    {"name": "no triplet", "args": [[0, 1, 1]], "expected": []},
    {"name": "all zeros", "args": [[0, 0, 0]], "expected": [[0, 0, 0]]},
    {"name": "many dups", "args": [[-2, 0, 0, 2, 2]], "expected": [[-2, 0, 2]]},
    {"name": "mixed", "args": [[-4, -2, -2, -2, 0, 1, 2, 2, 2, 3, 3, 4, 4, 6, 6]],
     "expected": [[-4, -2, 6], [-4, 0, 4], [-4, 1, 3], [-4, 2, 2], [-2, -2, 4], [-2, 0, 2]]},
]


def normalize(triplets):
    # Order-independent: sort within each triplet, then sort the collection.
    return sorted(tuple(sorted(t)) for t in triplets)


run_cases(
    threeSum, CASES,
    compare=lambda got, c, a: got is not None and normalize(got) == normalize(c["expected"]),
    fmt_input=lambda c: "nums={}".format(c["args"][0]),
)
