# notleetcode — Agent Problem Authoring Guide

This file tells an AI agent how to add a new problem to this repo. Read it fully before writing any files.

The canonical reference for every convention here is `problems/two-sum/`. When in doubt, copy from there.

---

## What a problem is

Each problem lives in `problems/<problem-id>/` and contains:

```
problems/<id>/
  meta.json          Required. Title, phase, difficulty, stack order.
  description.md     Required. The problem prompt rendered as markdown.
  starter.py         Required. Blank template shown in the editor (Python).
  starter.c          Required. Blank template shown in the editor (C).
  harness.py         Required. Hidden Python test driver.
  harness.c          Required. Hidden C test driver.
  solution_ref.py    Required. A correct Python solution (revealed on demand).
  solution_ref.c     Required. A correct C solution (revealed on demand).
```

All eight files are required for a complete problem. Do not create a problem directory without filling in all of them.

---

## meta.json

```json
{
  "title": "Human-readable title",
  "phase": 0,
  "phaseName": "Rust Removal",
  "difficulty": "Easy",
  "tags": ["array", "hash map"],
  "languages": ["python", "c"],
  "stackIndex": 1,
  "authored": true,
  "defaultLanguage": "python"
}
```

Field notes:
- `phase` / `phaseName`: see the phase table in README.md
- `stackIndex`: controls ordering in the sidebar. Pick the next available integer in the target phase. Run `node scripts/scaffold.js --list` (or inspect existing meta.json files) to find the current max.
- `authored`: must be `true` or the problem renders as a placeholder
- `languages`: always `["python", "c"]` for new problems
- `defaultLanguage`: `"python"` unless the problem is C-only by nature (e.g. pointer manipulation)

---

## description.md

Plain markdown. Structure:

```markdown
# Problem Title

One-paragraph problem statement.

### Example 1
```
Input:  ...
Output: ...
```

### Example 2
...

### Constraints
- ...

**Optimal complexity:** O(n) — one-line hint
```

Do not include hints, tags, or difficulty in the description — those are in meta.json. Write as you would see on LeetCode or a whiteboard.

---

## starter files

`starter.py` — minimal Python skeleton:

```python
from typing import List


def functionName(arg: type) -> type:
    pass
```

`starter.c` — minimal C skeleton:

```c
#include <stdio.h>

/* function signature matching what harness.c expects */
int functionName(int* arr, int n) {
    return 0;
}
```

No comments explaining what to do — the description.md is the prompt. No sample code, no hints.

---

## Python harness (harness.py)

The harness imports the user's function from `solution` and uses `run_cases` from `nlc`:

```python
from solution import functionName
from nlc import run_cases

CASES = [
    {"name": "example 1", "args": [arg1, arg2], "expected": result},
    {"name": "edge case", "args": [...], "expected": ...},
]

run_cases(functionName, CASES)
```

`run_cases` signature:
```python
run_cases(fn, cases, compare=None, fmt_input=None, show=repr)
```

- `compare(got, case, args) -> bool` — custom equality check. Use when order doesn't matter (e.g. sorted result) or when the expected value is complex.
- `fmt_input(case) -> str` — how the input is shown to the user. Default: repr-joined args.
- `show(value) -> str` — how expected/got are rendered. Default: `repr`.

Example with custom compare (order-insensitive indices):
```python
run_cases(
    twoSum, CASES,
    compare=lambda got, c, a: got is not None and sorted(got) == sorted(c["expected"]),
    fmt_input=lambda c: "nums={}, target={}".format(c["args"][0], c["args"][1]),
)
```

Write at least 5 test cases: the 2–3 from the description plus edge cases (empty input, single element, negatives, duplicates, large input).

---

## C harness (harness.c)

The C harness includes `nlc.h` and `solution.c`, then defines `main` with `nlc_begin()` ... `nlc_end()` wrapping one `nlc_case()` call per test.

```c
#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"
#include "solution.c"

int main(void) {
    nlc_begin();

    {
        /* set up inputs */
        int a[] = {2, 7, 11, 15};
        int rs = 0;
        nlc_capture_begin();
        int* r = twoSum(a, 4, 9, &rs);
        char* out = nlc_capture_end();

        int ok = r && rs == 2 && ((r[0]==0 && r[1]==1) || (r[0]==1 && r[1]==0));
        char gbuf[64], ebuf[32];
        if (r) nlc_fmt_ints(gbuf, sizeof gbuf, r, rs);
        else snprintf(gbuf, sizeof gbuf, "NULL");
        int exp[] = {0, 1};
        nlc_fmt_ints(ebuf, sizeof ebuf, exp, 2);

        nlc_case("example 1", ok, "nums=[2,7,11,15], target=9", ebuf, gbuf, out);
        free(r); free(out);
    }

    nlc_end();
    return 0;
}
```

`nlc.h` API:
- `nlc_begin()` — call once at the start of main
- `nlc_end()` — call once at the end of main
- `nlc_case(name, passed, input, expected, got, captured_stdout)` — one per test
- `nlc_fmt_ints(buf, cap, arr, n)` — formats an int array as `"[1, 2, 3]"`
- `nlc_capture_begin()` / `nlc_capture_end()` — capture stdout from the user's function so it shows up per-test instead of jumbled. `nlc_capture_end()` returns a `char*` that must be `free()`d.

All strings passed to `nlc_case` are JSON-escaped internally. Do not pre-escape them.

For problems where the C function signature returns void or writes into an output parameter, adjust accordingly — the two-sum harness is the reference.

---

## solution_ref files

A clean, correct solution with no extra comments. The same code a senior engineer would write in an interview. It is revealed to users when they click "Show reference solution" — write it accordingly.

---

## Verification (required before committing)

Run the reference solution through its own harness to confirm all tests pass:

**Python:**
```sh
cd /tmp && mkdir verify_test && cd verify_test
cp <repo>/problems/<id>/harness.py main.py
cp <repo>/problems/<id>/solution_ref.py solution.py
cp <repo>/problems/_lib/nlc.py nlc.py
python3 main.py
```

**C:**
```sh
cd /tmp && mkdir verify_c && cd verify_c
cp <repo>/problems/<id>/harness.c main.c
cp <repo>/problems/<id>/solution_ref.c solution.c
cp <repo>/problems/_lib/nlc.h nlc.h
gcc -std=c11 -O0 main.c -o prog -lm && ./prog
```

Both must output `__TESTS_BEGIN__` ... `__TESTS_END__` with `"passed": true` on every case. Fix any failures before committing.

---

## Committing a new problem

```sh
git add problems/<id>/
git commit -m "add problem: <title>"
git push
```

Do not modify any user files (`solutions/`, `progress/`), `server.js`, `web/`, or `scripts/` when adding a problem.
