# notleetcode

A local coding environment for interview prep — read a problem on the left, write your solution on the right, hit **Run**, and see pass/fail per test case. No account, no browser tab, no distractions.

Problems are weighted toward firmware and robotics interviews (grids, bit manipulation, from-scratch data structures), with both Python and C available on every problem.

---

## Prerequisites

| Tool | Why |
|------|-----|
| **Node.js** (any recent version) | Runs the server |
| **python3** | Runs Python solutions |
| **gcc** | Compiles C solutions |
| **git** | Syncing your work |

The editor (CodeMirror) and markdown renderer load from a CDN on first open, so you need internet once. After that the browser caches them.

---

## Setup

```sh
git clone <repo-url>
cd notleetcode
```

That's it — no `npm install`, no config files.

---

## Starting the app

You must set `NLC_USER` to your name before starting. This keeps your solutions and progress separate from everyone else's.

```sh
NLC_USER=tim ./start.sh
```

Then open **http://localhost:5151**.

To use a different port:

```sh
NLC_USER=tim PORT=8080 ./start.sh
```

To also open the browser automatically:

```sh
NLC_USER=tim ./start.sh --open
```

---

## Using it

The left sidebar is the problem list, grouped by phase. Click a problem or use **← Prev / Next →** to move through in order. The dot next to each problem shows your status:

| Dot | Meaning |
|-----|---------|
| grey | not started |
| amber | in progress |
| green | solved (all tests passed) |

Pick a language from the dropdown, write your solution, and click **Run Tests**. Once all tests pass the problem is marked solved — and stays solved even if a later experimental run fails.

A **Show reference solution** toggle reveals a worked answer when you want to compare. Your code autosaves as you type.

### Keyboard shortcuts

| Shortcut | Action |
|----------|--------|
| `Cmd/Ctrl + Enter` | Run tests |
| `Cmd/Ctrl + S` | Save now |
| `Cmd/Ctrl + /` | Toggle comment |
| `Tab` | Indent |
| `Shift + Tab` | Dedent |

---

## Saving and syncing your work

Your work lives in two places, both plain files in this repo:

- `solutions/<your-name>/` — your code, one file per problem per language
- `progress/<your-name>.json` — your status per problem

To save your progress and sync across machines:

```sh
git add solutions/<your-name>/ progress/<your-name>.json
git commit -m "progress"
git push
```

On another machine, `git pull` and start the server — your code and statuses come right back.

---

## Contributing a new problem

Problems live in `problems/<problem-id>/`. To add one, create that directory and fill in the required files. See [`CLAUDE.md`](CLAUDE.md) for the exact spec — it's written so an AI agent can author a complete problem from scratch.

If you're adding a problem manually (no agent), the short version:
1. Create `problems/<id>/` with `meta.json`, `description.md`, `starter.py`, `starter.c`, `harness.py`, `harness.c`, `solution_ref.py`, `solution_ref.c`
2. Run `node scripts/scaffold.js` to register it in the stack
3. Verify by running the reference solution through its own harness before pushing

---

## Problem stack

~80 problems across twelve phases:

| Phase | Theme |
|------:|-------|
| 0 | Rust Removal (warm-up fundamentals) |
| 1 | Arrays: Two Pointers & Sliding Window |
| 2 | Hashing |
| 3 | Stacks & Queues (incl. ring buffer) |
| 4 | Binary Search |
| 5 | Linked Lists |
| 6 | Matrices / 2D Arrays |
| 7 | Graphs & Grid Search (BFS / DFS) |
| 8 | Weighted Shortest Path (Dijkstra & A*) |
| 9 | Bit Manipulation |
| 10 | Domain Implementations (robotics + firmware, from scratch) |
| 11 | Tier-3 Minimum (trees, backtracking, intro DP) |

---

## Repo layout

```
problems/
  <problem-id>/
    meta.json          Title, phase, difficulty, stack order
    description.md     The prompt (rendered as markdown)
    starter.py / .c    Starter code shown in the editor
    harness.py / .c    Hidden test driver
    solution_ref.py/.c Reference solution (revealed on demand)
  _lib/
    nlc.py / nlc.h     Shared test-harness helpers
solutions/
  <your-name>/         Your saved code (commit this)
progress/
  <your-name>.json     Your status per problem (commit this)
server.js              Node server + code runner
web/                   Browser UI
scripts/
  scaffold.js          Creates placeholder problem directories
CLAUDE.md              Agent spec for authoring problems
```
