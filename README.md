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

If you're missing any you can ask Claude Code to install them.

---

## Setup

```sh
git clone <repo-url>
cd notleetcode
```

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

To keep the server running after you close your terminal, use a `screen` session:

```sh
screen -S notleetcode
NLC_USER=tim ./start.sh
```

Detach with `Ctrl+A D`. Reattach later with `screen -r notleetcode`. To stop the server, reattach and hit `Ctrl+C`.

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

Pick a language from the dropdown, write your solution, and click **Run Tests**. The status reflects your latest work: passing tests marks it solved; editing or a failed run puts it back in progress.

A **Show reference solution** toggle reveals a worked answer when you want to compare. Your code autosaves as you type.

### Keyboard shortcuts

| Shortcut | Action |
|----------|--------|
| `Cmd/Ctrl + Enter` | Run tests |
| `Cmd/Ctrl + S` | Save now |
| `Cmd/Ctrl + /` | Toggle comment |
| `Tab` | Indent |
| `Shift + Tab` | Dedent |

### Thursday interview prep

Open **Thursday prep · plan & recall tracker** in the sidebar, or visit
**http://localhost:5151/study.html**. The October 25–29 plan includes a prioritized
firmware route, daily timeboxes, shorter weekday routes, oral drills, and extensions.
The string-copy exercise adapts 42 Piscine C02 exercise 10 using the OpenBSD
`strlcpy` contract; links are in the question. Event handoff is labeled as an optional
custom simulation, and the core concurrency card uses an avr-libc shared-counter
example. The plan distinguishes practice references from interview reports.

Use **Open question** to see your latest code. **Start over** replaces it with the
starter for a fresh attempt. All edits autosave to `solutions/<your-name>/`; running
tests saves there too. There is one current solution per question and language.
**Saved to project** confirms the server accepted your code. Older browser-only
attempts are imported on opening the updated app only when there is no saved project
solution; conflicting drafts stay in the browser and do not replace your current work.
Git commits provide history when you want it.

Log **Stuck**, **Used help**, or **Independent**, plus minutes and a short mistake note.
Two consecutive independent attempts on different Los Angeles calendar days count as
**Retained**. Your ratings update **Due for recall**; the daily route stays fixed until
edited. **Undo last log** corrects a mistaken rating. Reviews are stored in
`progress/<your-name>.study.json` (the internal `cold` rating means Independent).

The app works standalone. It runs tests and schedules recall from your self-ratings;
it does not grade explanations, record speech, or automatically ask an AI to review
work. Ask Codex to review your saved code and mistake notes when you want coaching
and a revised plan. The latest code is available without keeping a special tab open.

Run `node scripts/test-study.js` to check the study plan, review persistence,
retention rules, and saving/running the latest solution. Run
`node scripts/test-editor.js` for save failures, language switching, queued edits,
and browser-draft recovery.

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
