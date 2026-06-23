from solution import groupAnagrams
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [["eat", "tea", "tan", "ate", "nat", "bat"]],
     "expected": [["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]},
    {"name": "empty string", "args": [[""]], "expected": [[""]]},
    {"name": "single", "args": [["a"]], "expected": [["a"]]},
    {"name": "no anagrams", "args": [["abc", "def", "ghi"]], "expected": [["abc"], ["def"], ["ghi"]]},
    {"name": "all same", "args": [["ab", "ba", "ab"]], "expected": [["ab", "ab", "ba"]]},
]


def normalize(groups):
    # Order-independent: sort within each group, then sort the list of groups.
    return sorted(sorted(g) for g in groups)


run_cases(
    groupAnagrams, CASES,
    compare=lambda got, c, a: got is not None and normalize(got) == normalize(c["expected"]),
    fmt_input=lambda c: "strs={}".format(c["args"][0]),
)
