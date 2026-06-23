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
4. **Language split:** Python for the DSA patterns (fast to write, embedded-interviewer-approved), **C for the FW-specific** items (marked `[C]`), C++ or Python for the robotics implementations (marked `[ROBO]`).
5. **Timebox:** ~30 min/problem. Stuck past that → read a hint, not the full solution. Struggle is the learning.

---

## Phase 0 — Rust Removal (raw fluency)
Knock these out fast; they're just to get your hands moving unassisted.
- [ ] Two Sum
- [ ] Valid Anagram
- [ ] Reverse String
- [ ] Best Time to Buy and Sell Stock
- [ ] Valid Parentheses

## Phase 1 — Arrays: Two Pointers & Sliding Window  *(Tier 1)*
- [ ] Two Sum II (Input Array Is Sorted)
- [ ] Valid Palindrome
- [ ] Container With Most Water
- [ ] 3Sum
- [ ] Maximum Subarray (Kadane's)
- [ ] Longest Substring Without Repeating Characters
- [ ] Minimum Size Subarray Sum
- [ ] Product of Array Except Self

## Phase 2 — Hashing  *(Tier 2, quick)*
- [ ] Contains Duplicate
- [ ] Group Anagrams
- [ ] Top K Frequent Elements
- [ ] Longest Consecutive Sequence

## Phase 3 — Stacks & Queues (incl. ring buffer)  *(Tier 2 + FW)*
- [ ] Valid Parentheses (revisit, monotonic mindset)
- [ ] Min Stack
- [ ] Daily Temperatures (monotonic stack)
- [ ] Implement Queue using Stacks
- [ ] Design Circular Queue (LC 622)
- [ ] `[C]` **Implement a ring buffer** (fixed array, head/tail, full-vs-empty handling, power-of-2 masking)

## Phase 4 — Binary Search  *(Tier 2; "search on answer" is high-leverage)*
- [ ] Binary Search
- [ ] Search in Rotated Sorted Array
- [ ] Find Minimum in Rotated Sorted Array
- [ ] Koko Eating Bananas (binary search on the answer)
- [ ] Capacity To Ship Packages Within D Days (search on the answer)

## Phase 5 — Linked Lists  *(Tier 2; FW pointer practice)*
- [ ] Reverse Linked List
- [ ] Linked List Cycle (Floyd's tortoise/hare)
- [ ] Merge Two Sorted Lists
- [ ] Remove Nth Node From End of List
- [ ] Reorder List

## Phase 6 — Matrices / 2D Arrays  *(Tier 1; robotics-flavored)*
- [ ] Rotate Image
- [ ] Spiral Matrix
- [ ] Set Matrix Zeroes
- [ ] Flood Fill (LC 733)

## Phase 7 — Graphs & Grid Search: BFS / DFS  *(Tier 1 — THE robotics tier)*
- [ ] Number of Islands
- [ ] Max Area of Island
- [ ] Rotting Oranges (multi-source BFS)
- [ ] Walls and Gates (multi-source BFS)
- [ ] Clone Graph
- [ ] Course Schedule (topological sort / cycle detection)
- [ ] Pacific Atlantic Water Flow

## Phase 8 — Weighted Shortest Path: Dijkstra & A*  *(Tier 1 — robotics core)*
- [ ] Network Delay Time (Dijkstra)
- [ ] Path With Minimum Effort
- [ ] Swim in Rising Water
- [ ] Cheapest Flights Within K Stops
- [ ] `[ROBO]` **Implement Dijkstra on a grid** (heap-based, 4/8-connectivity)
- [ ] `[ROBO]` **Implement A\* on an occupancy grid** (Manhattan + Euclidean heuristics, compare expansions vs Dijkstra)

## Phase 9 — Bit Manipulation  *(Tier 1 — FW core)*
- [ ] Number of 1 Bits (popcount)
- [ ] Counting Bits
- [ ] Reverse Bits
- [ ] Single Number
- [ ] Missing Number
- [ ] Sum of Two Integers (add without `+`)
- [ ] `[C]` **Set / clear / toggle / test a bit** (write the macros)
- [ ] `[C]` **Swap endianness** of a 32-bit integer
- [ ] `[C]` **Is power of two**, and **round up to next power of two**
- [ ] `[C]` **Pack / unpack a bit-field** (e.g. two 4-bit values in a byte — your Christmas-tree flash bug)

## Phase 10 — Domain Implementations (the main event — all from scratch, by hand)
This is where your value lives and where AI-generated code can't help you in the room.
Each doubles as proficiency maintenance and a resume talking point.

**Robotics `[ROBO]`**
- [ ] PID controller (with anti-windup and a fixed timestep)
- [ ] Moving-average filter, then a median filter
- [ ] 1D Kalman filter update (predict/update), then extend to 2D pose
- [ ] 2D coordinate-frame transform (rotation + translation, compose, invert)
- [ ] Bresenham line / line-of-sight check on a grid
- [ ] Point-in-polygon test
- [ ] Waypoint follower (pure-pursuit-style: pick lookahead, compute steering)
- [ ] (Stretch) RRT on a 2D map with obstacles

**Firmware `[C]`**
- [ ] `memcpy` and `memmove` (and explain why they differ)
- [ ] Fixed-point multiply (Q-format, handle the shift and overflow)
- [ ] Debounce state machine (FSM, no `delay()`)
- [ ] Simple serial **packet framing parser** (start byte, length, payload, checksum)
- [ ] CRC8 / checksum
- [ ] (Stretch) A tiny cooperative scheduler (run N tasks at different rates off one tick)

## Phase 11 — Tier-3 Minimum (learn the shape, do NOT rabbit-hole)
Enough to not freeze if one appears. Stop after these; skip hard DP entirely.
- [ ] Invert Binary Tree
- [ ] Maximum Depth of Binary Tree
- [ ] Binary Tree Level Order Traversal (BFS)
- [ ] Validate Binary Search Tree
- [ ] Subsets (backtracking template)
- [ ] Permutations
- [ ] Climbing Stairs (DP intro)
- [ ] Coin Change (DP)
- [ ] House Robber (DP)

---

## How to use this
- Phases 1–9 rebuild fluency and the recognition patterns; ~1 problem/day puts you through them in ~6 weeks.
- Phase 10 is the differentiator — slower, meatier, ~2–4 per week, and the most interview-representative for your roles.
- Phase 11 is a weekend of exposure, not a campaign.
- Backbone reference (free, pattern-ordered): NeetCode 150. FW reference: the `theEmbeddedNewTestament` repo and the Enlightenment777 embedded-questions list.
