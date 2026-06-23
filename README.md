# notleetcode

A local, LeetCode/CoderPad-style practice environment for working through a
fixed, ordered stack of firmware + robotics interview problems. Run it on any
machine, solve problems in the browser, run them against hidden test cases, and
commit your progress to git so you can pick up where you left off elsewhere.

The problem stack lives in [`coding_practice_stack.md`](coding_practice_stack.md).

## Requirements

- **Node.js** (any recent version) — runs the server. No npm packages needed.
- **python3** — to run/test the Python problems (Phases 0–9, 11, robotics).
- **gcc / g++** — to compile/test the `[C]` firmware problems.

First page load pulls the CodeMirror editor + marked from a CDN, so an internet
connection is needed the first time (after that the browser caches them).

## Run it

```sh
node server.js
# then open http://localhost:5151
```

Set a different port with `PORT=8080 node server.js`.

## How it works

- **Stack order** is the left sidebar, top to bottom, grouped by phase. A colored
  dot shows status: grey = not started, amber = in progress, green = solved.
- **Run Tests** (or `Cmd/Ctrl+Enter`) compiles+runs your code against the
  problem's hidden test harness and shows per-case pass/fail. Passing every case
  marks the problem solved.
- **Your code autosaves** to `solutions/<id>.<ext>` as you type, and progress is
  recorded in `progress.json`.
- **Sync across machines:** commit `progress.json` and `solutions/` and push;
  pull on the next machine.

```sh
git add progress.json solutions/ && git commit -m "progress" && git push
```

## Repo layout

```
server.js              # zero-dependency Node server + code runner
web/                   # the UI (index.html, app.js, style.css)
problems/<id>/
  meta.json            # title, phase, difficulty, tags, languages, order
  description.md       # the prompt (rendered as markdown)
  starter.py|.c        # starter code shown in the editor
  harness.py|.c        # hidden test driver (prints results between markers)
  solution_ref.py|.c   # reference solution (revealed via "Show solution")
scripts/scaffold.js    # canonical ordered stack; creates placeholder problems
solutions/             # your saved code (committed)
progress.json          # your status per problem (committed)
```

## Languages

Every problem offers both **Python** and **C** in the editor's language dropdown,
regardless of what the markdown stack suggests. The server forces this; it does
not read per-problem language lists. Each language has its own starter, hidden
test harness, and reference solution.

## Authoring more problems

Phases 0 and 1 are fully authored in both Python and C. The remaining problems
exist as placeholders (visible in the UI, marked unauthored). To author one,
fill in its `problems/<id>/` directory:

1. Set `"authored": true` in `meta.json`.
2. Write `description.md`, `starter.py` / `starter.c`, and reference solutions
   `solution_ref.py` / `solution_ref.c`.
3. Write the test harness for each language. Harnesses use the shared helper in
   `problems/_lib/` (`nlc.py` / `nlc.h`), which the server copies into the run
   sandbox automatically. They handle per-test stdout capture, runtime-error
   tracebacks, and the `__TESTS_BEGIN__` / `__TESTS_END__` JSON protocol.

   - **Python** (`harness.py`, run as `main.py`):
     ```python
     from solution import myFunc
     from nlc import run_cases
     CASES = [{"name": "...", "args": [...], "expected": ...}]
     run_cases(myFunc, CASES)   # optional: compare=..., fmt_input=...
     ```
   - **C** (`harness.c`, compiled as `main.c` which `#include "solution.c"`):
     ```c
     #include "nlc.h"
     #include "solution.c"
     int main(void){ nlc_begin(); /* nlc_case(...) per test */ nlc_end(); }
     ```

   See `problems/two-sum/` for the full pattern in both languages.

Re-run `node scripts/scaffold.js` any time to create placeholders for newly added
stack entries; it never overwrites existing problems.
