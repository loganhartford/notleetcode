# Coding Practice Stack — Firmware + Robotics Interview Prep

A single ordered list. Work top to bottom. Ignore the rest of LeetCode.

Weighted for FW/robotics interviews, not FAANG: heavy on graph/grid search, bit manipulation,
arrays, and from-scratch algorithm implementations; light on DP/trees/backtracking.

---

## The Rules (read once)

1. **Solve unassisted.** No Copilot, no AI autocomplete, plain editor. You generate code at work, so the
   tested muscle — writing correct code live and explaining it — is the one that's atrophied. The doing-it-by-hand
   *is* the point here. Use AI only **after** you've solved it (review, optimize, generate the next variation).
2. **The loop per problem:** attempt → test your own edge cases → redo from memory → say the time/space complexity out loud → move on.
3. **Flashcard the pattern, not the problem.** After each phase, add a recognition card: "shortest path on a grid → BFS / Dijkstra / A*", "pair summing to target → hash map or two pointers." That deck *is* the framework you asked for.
4. **Timebox:** ~30 min/problem. Stuck past that → read a hint, not the full solution. Struggle is the learning.

## Language tags
Each problem has a suggested language. The rule behind the choice:
- **`lang:Python`** — pure-algorithm fluency where the language is irrelevant, *and* anything needing hash maps, sets, queues, or a heap (graphs, shortest path). Fastest to write, so you focus on the algorithm; also the embedded-interviewer-approved default for DSA rounds.
- **`lang:C`** — the problem *is* about bits, pointers, or fixed-size buffers. C is the skill being tested. Never used for container-heavy problems (you'd waste the session hand-rolling a hash table).
- **`[author]`** — no LeetCode equivalent exists: the build tool should **author** the spec + tests, not search the web for them.

---

## Phase 0 — Rust Removal (raw fluency)
Knock these out fast; they're just to get your hands moving unassisted.
- [ ] `lang:Python` Two Sum
- [ ] `lang:Python` Valid Anagram
- [ ] `lang:Python` Reverse String
- [ ] `lang:Python` Best Time to Buy and Sell Stock
- [ ] `lang:Python` Valid Parentheses

## Phase 1 — Arrays: Two Pointers & Sliding Window  *(Tier 1)*
- [ ] `lang:Python` Two Sum II (Input Array Is Sorted)
- [ ] `lang:Python` Valid Palindrome
- [ ] `lang:Python` Container With Most Water
- [ ] `lang:Python` 3Sum
- [ ] `lang:Python` Maximum Subarray (Kadane's)
- [ ] `lang:Python` Longest Substring Without Repeating Characters
- [ ] `lang:Python` Minimum Size Subarray Sum
- [ ] `lang:Python` Product of Array Except Self

## Phase 2 — Hashing  *(Tier 2, quick — never C)*
- [ ] `lang:Python` Contains Duplicate
- [ ] `lang:Python` Group Anagrams
- [ ] `lang:Python` Top K Frequent Elements
- [ ] `lang:Python` Longest Consecutive Sequence

## Phase 3 — Stacks & Queues (incl. ring buffer)  *(Tier 2 + FW)*
- [ ] `lang:Python` Valid Parentheses (revisit, monotonic mindset)
- [ ] `lang:Python` Min Stack
- [ ] `lang:Python` Daily Temperatures (monotonic stack)
- [ ] `lang:Python` Implement Queue using Stacks
- [ ] `lang:C` Design Circular Queue (LC 622) — fixed array + indices, good C structure practice
- [ ] `lang:C` `[author]` **Implement a ring buffer** (fixed array, head/tail, full-vs-empty handling, power-of-2 masking)

## Phase 4 — Binary Search  *(Tier 2; "search on answer" is high-leverage)*
*(All Python; binary search is also C-fair if you want extra C reps — it's just arrays and indices.)*
- [ ] `lang:Python` Binary Search
- [ ] `lang:Python` Search in Rotated Sorted Array
- [ ] `lang:Python` Find Minimum in Rotated Sorted Array
- [ ] `lang:Python` Koko Eating Bananas (binary search on the answer)
- [ ] `lang:Python` Capacity To Ship Packages Within D Days (search on the answer)

## Phase 5 — Linked Lists  *(Tier 2; FW pointer practice — do these in C, not Python)*
- [ ] `lang:C` Reverse Linked List
- [ ] `lang:C` Linked List Cycle (Floyd's tortoise/hare)
- [ ] `lang:C` Merge Two Sorted Lists
- [ ] `lang:C` Remove Nth Node From End of List
- [ ] `lang:C` Reorder List

## Phase 6 — Matrices / 2D Arrays  *(Tier 1; robotics-flavored)*
- [ ] `lang:Python` Rotate Image
- [ ] `lang:Python` Spiral Matrix
- [ ] `lang:Python` Set Matrix Zeroes
- [ ] `lang:Python` Flood Fill (LC 733)

## Phase 7 — Graphs & Grid Search: BFS / DFS  *(Tier 1 — THE robotics tier; Python for the hash sets/queues)*
- [ ] `lang:Python` Number of Islands
- [ ] `lang:Python` Max Area of Island
- [ ] `lang:Python` Rotting Oranges (multi-source BFS)
- [ ] `lang:Python` Walls and Gates (multi-source BFS)
- [ ] `lang:Python` Clone Graph
- [ ] `lang:Python` Course Schedule (topological sort / cycle detection)
- [ ] `lang:Python` Pacific Atlantic Water Flow

## Phase 8 — Weighted Shortest Path: Dijkstra & A*  *(Tier 1 — robotics core; Python heapq for the priority queue)*
- [ ] `lang:Python` Network Delay Time (Dijkstra)
- [ ] `lang:Python` Path With Minimum Effort
- [ ] `lang:Python` Swim in Rising Water
- [ ] `lang:Python` Cheapest Flights Within K Stops
- [ ] `lang:Python` `[author]` **Implement Dijkstra on a grid** (heap-based, 4/8-connectivity)
- [ ] `lang:Python` `[author]` **Implement A\* on an occupancy grid** (Manhattan + Euclidean heuristics, compare expansions vs Dijkstra)

## Phase 9 — Bit Manipulation  *(Tier 1 — FW core; all C, C's home turf)*
- [ ] `lang:C` Number of 1 Bits (popcount)
- [ ] `lang:C` Counting Bits
- [ ] `lang:C` Reverse Bits
- [ ] `lang:C` Single Number
- [ ] `lang:C` Missing Number
- [ ] `lang:C` Sum of Two Integers (add without `+`)
- [ ] `lang:C` `[author]` **Set / clear / toggle / test a bit** (write the macros)
- [ ] `lang:C` `[author]` **Swap endianness** of a 32-bit integer
- [ ] `lang:C` `[author]` **Is power of two**, and **round up to next power of two**
- [ ] `lang:C` `[author]` **Pack / unpack a bit-field** (e.g. two 4-bit values in a byte — your Christmas-tree flash bug)

## Phase 10 — Domain Implementations (the main event — all from scratch, by hand, all `[author]`)
This is where your value lives and where AI-generated code can't help you in the room.
Each doubles as proficiency maintenance and a resume talking point.

**Robotics** *(Python — clean for math-y algorithm work)*
- [ ] `lang:Python` `[author]` PID controller (with anti-windup and a fixed timestep)
- [ ] `lang:Python` `[author]` Moving-average filter, then a median filter
- [ ] `lang:Python` `[author]` 1D Kalman filter update (predict/update), then extend to 2D pose
- [ ] `lang:Python` `[author]` 2D coordinate-frame transform (rotation + translation, compose, invert)
- [ ] `lang:Python` `[author]` Bresenham line / line-of-sight check on a grid
- [ ] `lang:Python` `[author]` Point-in-polygon test
- [ ] `lang:Python` `[author]` Waypoint follower (pure-pursuit-style: pick lookahead, compute steering)
- [ ] `lang:Python` `[author]` (Stretch) RRT on a 2D map with obstacles

**Firmware** *(C — these are the skill)*
- [ ] `lang:C` `[author]` `memcpy` and `memmove` (and explain why they differ)
- [ ] `lang:C` `[author]` Fixed-point multiply (Q-format, handle the shift and overflow)
- [ ] `lang:C` `[author]` Debounce state machine (FSM, no `delay()`)
- [ ] `lang:C` `[author]` Simple serial **packet framing parser** (start byte, length, payload, checksum)
- [ ] `lang:C` `[author]` CRC8 / checksum
- [ ] `lang:C` `[author]` (Stretch) A tiny cooperative scheduler (run N tasks at different rates off one tick)

## Phase 11 — Tier-3 Minimum (learn the shape, do NOT rabbit-hole)
Enough to not freeze if one appears. Stop after these; skip hard DP entirely.
- [ ] `lang:Python` Invert Binary Tree
- [ ] `lang:Python` Maximum Depth of Binary Tree
- [ ] `lang:Python` Binary Tree Level Order Traversal (BFS)
- [ ] `lang:Python` Validate Binary Search Tree
- [ ] `lang:Python` Subsets (backtracking template)
- [ ] `lang:Python` Permutations
- [ ] `lang:Python` Climbing Stairs (DP intro)
- [ ] `lang:Python` Coin Change (DP)
- [ ] `lang:Python` House Robber (DP)

---

## How to use this
- Phases 1–9 rebuild fluency and the recognition patterns; ~1 problem/day puts you through them in ~6 weeks.
- Phase 10 is the differentiator — slower, meatier, ~2–4 per week, and the most interview-representative for your roles.
- Phase 11 is a weekend of exposure, not a campaign.
- Toolchains needed: Python 3, gcc (C).
- Backbone reference (free, pattern-ordered): NeetCode 150. FW reference: the `theEmbeddedNewTestament` repo and the Enlightenment777 embedded-questions list.

## Language summary
- **Python** — Phases 0, 1, 2, 3 (most), 4, 6, 7, 8, 10-Robotics, 11. Pure-algorithm fluency plus everything container/heap-heavy.
- **C** — Phase 5 (linked lists), Phase 9 (bits), the FW implementations in Phase 10, and the two structure-builds in Phase 3. Bits, pointers, buffers.