"""Shared Python test-harness helper, injected into the run sandbox as nlc.py.

A problem's harness.py imports `run_cases` and calls it with the user's
function and a list of cases. This centralizes:
  - per-case stdout capture (so each test's prints stay with that test),
  - full traceback capture on runtime errors,
  - the __TESTS_BEGIN__ / __TESTS_END__ JSON protocol the server parses.
"""
import json
import copy
import io
import traceback
from contextlib import redirect_stdout

_BEGIN = "__TESTS_BEGIN__"
_END = "__TESTS_END__"


def run_cases(fn, cases, compare=None, fmt_input=None, show=repr):
    """Run `fn` against `cases` and emit results.

    Each case is a dict: {"name", "args": [...], "expected", optional "input"}.
    compare(got, case, args) -> bool   (default: got == case["expected"])
    fmt_input(case) -> str             (default: repr-joined args)
    show(value) -> str                 (how expected/got are rendered)
    """
    results = []
    for c in cases:
        args = copy.deepcopy(c["args"])
        if "input" in c:
            inp = c["input"]
        elif fmt_input is not None:
            inp = fmt_input(c)
        else:
            inp = ", ".join(repr(a) for a in c["args"])

        buf = io.StringIO()
        try:
            with redirect_stdout(buf):
                got = fn(*args)
            ok = (got == c["expected"]) if compare is None else bool(compare(got, c, args))
            res = {
                "name": c.get("name", ""),
                "passed": bool(ok),
                "input": inp,
                "expected": show(c["expected"]),
                "got": show(got),
            }
        except Exception:
            res = {
                "name": c.get("name", ""),
                "passed": False,
                "input": inp,
                "expected": show(c["expected"]),
                "got": "(runtime error)",
                "trace": traceback.format_exc(),
            }

        out = buf.getvalue()
        if out:
            res["stdout"] = out
        results.append(res)

    print(_BEGIN)
    print(json.dumps({"results": results}))
    print(_END)
