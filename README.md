# notleetcode

A local, **LeetCode / CoderPad-style coding environment** that runs entirely on
your own machine. It walks you, top to bottom, through a fixed stack of firmware
and robotics interview problems: read the prompt on the left, write a solution on
the right, hit **Run**, and get per-test pass/fail against hidden test cases —
in **Python or C**, your choice, on every problem.

Your progress and your code are saved as plain files in the repo, so you can stop
on one machine, `git push`, and pick up exactly where you left off on another.

The full, ordered problem list lives in
[`coding_practice_stack.md`](coding_practice_stack.md).

---

## Features

- **Two-pane workspace** — rendered problem description beside a real code editor.
- **Run against hidden tests** — each problem ships a test harness; you see a
  pass/fail card per case with `input` / `expected` / `got`.
- **Python *and* C on every problem** — switch languages from a dropdown; each has
  its own starter code, tests, and reference solution.
- **Real per-test output** — `print` / `printf` from each test is captured and
  shown *with that test*, not jumbled together.
- **Full tracebacks** — a runtime error shows the complete stack trace on the
  failing case, not a one-line summary.
- **VSCode-like editor** — soft tabs (Tab inserts spaces), `Cmd/Ctrl + /` to
  comment, bracket matching/closing, no noisy indentation highlighting.
- **Progress tracking** — a status dot per problem (not started / in progress /
  solved) and prev/next navigation through the stack.
- **Git-friendly persistence** — code and progress are committable files; sync
  across machines with ordinary `git push` / `git pull`.
- **Zero npm dependencies** — the server is plain Node.

---

## Requirements

| Tool | Why |
|------|-----|
| **Node.js** (any recent version) | Runs the server. No packages to install. |
| **python3** | Runs/tests Python solutions. |
| **gcc** | Compiles/tests C solutions. |

The editor (CodeMirror) and the markdown renderer load from a CDN on first page
load, so you need internet **once**; after that the browser caches them.

---

## Quick start

```sh
node server.js
```

Then open **http://localhost:5151**.

Use a different port with `PORT=8080 node server.js`.

---

## Using it

The left sidebar is the **stack**, grouped by phase and ordered top to bottom.
Click any problem to open it, or use the **← Prev / Next →** buttons to move
through the stack in order. The colored dot next to each problem is its status:

| Dot | Meaning |
|-----|---------|
| grey | not started |
| amber | in progress (you've edited or run it) |
| green | solved (all tests passed at least once) |

Pick a language from the dropdown, write your solution, and **Run Tests**. Every
test that passes turns the case green; pass them all and the problem is marked
solved. Once solved, a problem stays solved even if a later experimental run
fails — so you can keep tinkering without losing the checkmark.

A **Show reference solution** toggle reveals a worked answer once you want to
compare. Your code autosaves as you type.

### Keyboard shortcuts

| Shortcut | Action |
|----------|--------|
| `Cmd/Ctrl + Enter` | Run tests |
| `Cmd/Ctrl + S` | Save now |
| `Cmd/Ctrl + /` | Toggle line comment |
| `Tab` | Indent (inserts spaces) |
| `Shift + Tab` | Dedent |

---

## How your work is saved

Your work lives in two places, both **data files separate from the app code**:

- `solutions/<problem-id>.<ext>` — the code you've written, per problem per
  language, autosaved.
- `progress.json` — your status for each problem.

Because these are just files, updating the app (server, UI, problem definitions)
never touches them. To sync across machines or keep a safe restore point, commit
them:

```sh
git add solutions/ progress.json && git commit -m "progress" && git push
```

On another machine, `git pull` and run the server — your code and statuses come
right back.

---

## The stack

~80 problems across twelve phases, weighted for firmware/robotics interviews
(heavy on grids/graphs, bit manipulation, and from-scratch implementations).

| Phase | Theme |
|------:|-------|
| 0 | Rust Removal (warm-up fundamentals) |
| 1 | Arrays: Two Pointers & Sliding Window |
| 2 | Hashing |
| 3 | Stacks & Queues (incl. ring buffer) |
| 4 | Binary Search (incl. "search on the answer") |
| 5 | Linked Lists |
| 6 | Matrices / 2D Arrays |
| 7 | Graphs & Grid Search (BFS / DFS) |
| 8 | Weighted Shortest Path (Dijkstra & A*) |
| 9 | Bit Manipulation |
| 10 | Domain Implementations (robotics + firmware, from scratch) |
| 11 | Tier-3 Minimum (trees, backtracking, intro DP) |

**Phases 0 and 1 are fully authored** (descriptions, tests, and reference
solutions in both Python and C). The rest appear in the UI as placeholders and
are filled in over time.

---

## Project layout

```
server.js              Zero-dependency Node server + code runner
web/                   The browser UI
  index.html
  app.js
  style.css
problems/
  <problem-id>/
    meta.json          Title, phase, difficulty, tags, stack order
    description.md     The prompt (rendered as markdown)
    starter.py / .c    Starter code shown in the editor
    harness.py / .c    Hidden test driver
    solution_ref.py/.c Reference solution (revealed on demand)
  _lib/
    nlc.py / nlc.h     Shared test-harness helpers, injected at run time
scripts/
  scaffold.js          Canonical ordered stack; creates placeholder problems
solutions/             Your saved code  (commit this)
progress.json          Your status per problem  (commit this)
coding_practice_stack.md   The source-of-truth problem list
```

---

## How code execution works

1. The browser sends your code + language to the server (`POST /api/run`).
2. The server writes your code, the problem's harness, and the shared helper
   (`nlc.py` / `nlc.h`) into a temporary sandbox directory.
3. **Python** runs as `python3 main.py`; **C** compiles `gcc main.c` (the harness
   `#include`s your `solution.c`) and runs the binary.
4. The harness prints a JSON results blob between `__TESTS_BEGIN__` and
   `__TESTS_END__` markers; the server parses it and returns per-test results,
   captured stdout, and any compile/runtime errors.
5. Runs have a timeout, and the sandbox is deleted afterward.

Every problem offers both **Python** and **C** in the dropdown, regardless of
what the source stack suggests — the server forces this and does not read
per-problem language lists.

---

## Authoring more problems

To turn a placeholder into a real problem, fill in its `problems/<id>/` directory:

1. Set `"authored": true` in `meta.json`.
2. Write `description.md`, `starter.py` / `starter.c`, and the reference
   solutions `solution_ref.py` / `solution_ref.c`.
3. Write the test harness for each language. Harnesses use the shared helper in
   `problems/_lib/`, which the server copies into the sandbox automatically — it
   handles per-test stdout capture, tracebacks, and the JSON protocol.

   **Python** (`harness.py`, run as `main.py`):
   ```python
   from solution import myFunc
   from nlc import run_cases

   CASES = [{"name": "example", "args": [[2, 7, 11, 15], 9], "expected": [0, 1]}]
   run_cases(myFunc, CASES)   # optional: compare=..., fmt_input=...
   ```

   **C** (`harness.c`, compiled as `main.c` which `#include`s your solution):
   ```c
   #include "nlc.h"
   #include "solution.c"

   int main(void) {
       nlc_begin();
       /* nlc_case(name, passed, input, expected, got, captured_stdout) per test */
       nlc_end();
       return 0;
   }
   ```

`problems/two-sum/` is the reference example in both languages. Always verify a
new problem by running its reference solution through its own harness before
considering it done.

Re-run `node scripts/scaffold.js` any time to create placeholders for newly added
stack entries; it skips anything that already exists and never overwrites your
work.
