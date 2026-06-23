#!/usr/bin/env node
'use strict';

// Canonical, ordered definition of the whole practice stack.
// Running this creates a problems/<id>/ directory for every entry that does
// not already exist. It never overwrites hand-authored problems (it skips any
// directory that already has a meta.json), so it is safe to re-run.

const fs = require('fs');
const path = require('path');

const PROBLEMS_DIR = path.join(__dirname, '..', 'problems');

const PHASE_NAMES = {
  0: 'Rust Removal',
  1: 'Two Pointers & Sliding Window',
  2: 'Hashing',
  3: 'Stacks & Queues',
  4: 'Binary Search',
  5: 'Linked Lists',
  6: 'Matrices / 2D Arrays',
  7: 'Graphs & Grid Search',
  8: 'Weighted Shortest Path',
  9: 'Bit Manipulation',
  10: 'Domain Implementations',
  11: 'Tier-3 Minimum',
};

// [phase, id, title, difficulty, languages, tags]
const STACK = [
  [0, 'two-sum', 'Two Sum', 'Easy', ['python'], ['array', 'hash map']],
  [0, 'valid-anagram', 'Valid Anagram', 'Easy', ['python'], ['string', 'hash map']],
  [0, 'reverse-string', 'Reverse String', 'Easy', ['python'], ['two pointers', 'string']],
  [0, 'best-time-to-buy-sell-stock', 'Best Time to Buy and Sell Stock', 'Easy', ['python'], ['array', 'greedy']],
  [0, 'valid-parentheses', 'Valid Parentheses', 'Easy', ['python'], ['stack', 'string']],

  [1, 'two-sum-ii', 'Two Sum II (Input Array Is Sorted)', 'Medium', ['python'], ['two pointers']],
  [1, 'valid-palindrome', 'Valid Palindrome', 'Easy', ['python'], ['two pointers', 'string']],
  [1, 'container-with-most-water', 'Container With Most Water', 'Medium', ['python'], ['two pointers', 'greedy']],
  [1, 'three-sum', '3Sum', 'Medium', ['python'], ['two pointers', 'sorting']],
  [1, 'maximum-subarray', "Maximum Subarray (Kadane's)", 'Medium', ['python'], ['array', 'dp']],
  [1, 'longest-substring-without-repeating', 'Longest Substring Without Repeating Characters', 'Medium', ['python'], ['sliding window', 'hash map']],
  [1, 'minimum-size-subarray-sum', 'Minimum Size Subarray Sum', 'Medium', ['python'], ['sliding window']],
  [1, 'product-of-array-except-self', 'Product of Array Except Self', 'Medium', ['python'], ['array', 'prefix']],

  [2, 'contains-duplicate', 'Contains Duplicate', 'Easy', ['python'], ['hash set']],
  [2, 'group-anagrams', 'Group Anagrams', 'Medium', ['python'], ['hash map', 'string']],
  [2, 'top-k-frequent', 'Top K Frequent Elements', 'Medium', ['python'], ['heap', 'bucket sort']],
  [2, 'longest-consecutive-sequence', 'Longest Consecutive Sequence', 'Medium', ['python'], ['hash set']],

  [3, 'valid-parentheses-revisit', 'Valid Parentheses (revisit, monotonic mindset)', 'Easy', ['python'], ['stack']],
  [3, 'min-stack', 'Min Stack', 'Medium', ['python'], ['stack', 'design']],
  [3, 'daily-temperatures', 'Daily Temperatures', 'Medium', ['python'], ['monotonic stack']],
  [3, 'queue-using-stacks', 'Implement Queue using Stacks', 'Easy', ['python'], ['stack', 'queue', 'design']],
  [3, 'design-circular-queue', 'Design Circular Queue', 'Medium', ['python'], ['array', 'design', 'ring buffer']],
  [3, 'ring-buffer', '[C] Implement a ring buffer', 'Medium', ['c'], ['firmware', 'ring buffer']],

  [4, 'binary-search', 'Binary Search', 'Easy', ['python'], ['binary search']],
  [4, 'search-rotated-sorted-array', 'Search in Rotated Sorted Array', 'Medium', ['python'], ['binary search']],
  [4, 'find-min-rotated-sorted-array', 'Find Minimum in Rotated Sorted Array', 'Medium', ['python'], ['binary search']],
  [4, 'koko-eating-bananas', 'Koko Eating Bananas', 'Medium', ['python'], ['binary search on answer']],
  [4, 'ship-packages-d-days', 'Capacity To Ship Packages Within D Days', 'Medium', ['python'], ['binary search on answer']],

  [5, 'reverse-linked-list', 'Reverse Linked List', 'Easy', ['python'], ['linked list']],
  [5, 'linked-list-cycle', 'Linked List Cycle', 'Easy', ['python'], ['linked list', 'two pointers']],
  [5, 'merge-two-sorted-lists', 'Merge Two Sorted Lists', 'Easy', ['python'], ['linked list']],
  [5, 'remove-nth-from-end', 'Remove Nth Node From End of List', 'Medium', ['python'], ['linked list', 'two pointers']],
  [5, 'reorder-list', 'Reorder List', 'Medium', ['python'], ['linked list']],

  [6, 'rotate-image', 'Rotate Image', 'Medium', ['python'], ['matrix']],
  [6, 'spiral-matrix', 'Spiral Matrix', 'Medium', ['python'], ['matrix']],
  [6, 'set-matrix-zeroes', 'Set Matrix Zeroes', 'Medium', ['python'], ['matrix']],
  [6, 'flood-fill', 'Flood Fill', 'Easy', ['python'], ['dfs', 'bfs', 'grid']],

  [7, 'number-of-islands', 'Number of Islands', 'Medium', ['python'], ['dfs', 'bfs', 'grid']],
  [7, 'max-area-of-island', 'Max Area of Island', 'Medium', ['python'], ['dfs', 'bfs', 'grid']],
  [7, 'rotting-oranges', 'Rotting Oranges', 'Medium', ['python'], ['bfs', 'multi-source', 'grid']],
  [7, 'walls-and-gates', 'Walls and Gates', 'Medium', ['python'], ['bfs', 'multi-source', 'grid']],
  [7, 'clone-graph', 'Clone Graph', 'Medium', ['python'], ['graph', 'dfs', 'bfs']],
  [7, 'course-schedule', 'Course Schedule', 'Medium', ['python'], ['topological sort', 'cycle detection']],
  [7, 'pacific-atlantic', 'Pacific Atlantic Water Flow', 'Medium', ['python'], ['dfs', 'bfs', 'grid']],

  [8, 'network-delay-time', 'Network Delay Time', 'Medium', ['python'], ['dijkstra', 'graph']],
  [8, 'path-with-minimum-effort', 'Path With Minimum Effort', 'Medium', ['python'], ['dijkstra', 'grid']],
  [8, 'swim-in-rising-water', 'Swim in Rising Water', 'Hard', ['python'], ['dijkstra', 'grid']],
  [8, 'cheapest-flights-k-stops', 'Cheapest Flights Within K Stops', 'Medium', ['python'], ['bellman-ford', 'bfs']],
  [8, 'dijkstra-grid', '[ROBO] Implement Dijkstra on a grid', 'Medium', ['python'], ['robotics', 'dijkstra', 'grid']],
  [8, 'astar-grid', '[ROBO] Implement A* on an occupancy grid', 'Medium', ['python'], ['robotics', 'a-star', 'grid']],

  [9, 'number-of-1-bits', 'Number of 1 Bits', 'Easy', ['python'], ['bit manipulation']],
  [9, 'counting-bits', 'Counting Bits', 'Easy', ['python'], ['bit manipulation', 'dp']],
  [9, 'reverse-bits', 'Reverse Bits', 'Easy', ['python'], ['bit manipulation']],
  [9, 'single-number', 'Single Number', 'Easy', ['python'], ['bit manipulation', 'xor']],
  [9, 'missing-number', 'Missing Number', 'Easy', ['python'], ['bit manipulation', 'math']],
  [9, 'sum-of-two-integers', 'Sum of Two Integers', 'Medium', ['python'], ['bit manipulation']],
  [9, 'bit-macros', '[C] Set / clear / toggle / test a bit', 'Easy', ['c'], ['firmware', 'bit manipulation']],
  [9, 'swap-endianness', '[C] Swap endianness of a 32-bit integer', 'Easy', ['c'], ['firmware', 'bit manipulation']],
  [9, 'power-of-two', '[C] Is power of two, round up to next power of two', 'Easy', ['c'], ['firmware', 'bit manipulation']],
  [9, 'bit-field-pack', '[C] Pack / unpack a bit-field', 'Medium', ['c'], ['firmware', 'bit manipulation']],

  [10, 'pid-controller', '[ROBO] PID controller (with anti-windup)', 'Medium', ['python'], ['robotics', 'control']],
  [10, 'moving-average-median-filter', '[ROBO] Moving-average filter, then median filter', 'Medium', ['python'], ['robotics', 'filter']],
  [10, 'kalman-filter-1d', '[ROBO] 1D Kalman filter (predict/update)', 'Medium', ['python'], ['robotics', 'estimation']],
  [10, 'coordinate-frame-transform', '[ROBO] 2D coordinate-frame transform', 'Medium', ['python'], ['robotics', 'geometry']],
  [10, 'bresenham-line', '[ROBO] Bresenham line / line-of-sight on a grid', 'Medium', ['python'], ['robotics', 'geometry', 'grid']],
  [10, 'point-in-polygon', '[ROBO] Point-in-polygon test', 'Medium', ['python'], ['robotics', 'geometry']],
  [10, 'pure-pursuit', '[ROBO] Waypoint follower (pure pursuit)', 'Medium', ['python'], ['robotics', 'control']],
  [10, 'rrt', '[ROBO] RRT on a 2D map with obstacles', 'Hard', ['python'], ['robotics', 'planning']],
  [10, 'memcpy-memmove', '[C] memcpy and memmove', 'Medium', ['c'], ['firmware']],
  [10, 'fixed-point-multiply', '[C] Fixed-point multiply (Q-format)', 'Medium', ['c'], ['firmware', 'math']],
  [10, 'debounce-fsm', '[C] Debounce state machine (FSM)', 'Medium', ['c'], ['firmware', 'fsm']],
  [10, 'packet-framing-parser', '[C] Serial packet framing parser', 'Medium', ['c'], ['firmware', 'parsing']],
  [10, 'crc8', '[C] CRC8 / checksum', 'Medium', ['c'], ['firmware']],
  [10, 'cooperative-scheduler', '[C] Tiny cooperative scheduler', 'Hard', ['c'], ['firmware', 'rtos']],

  [11, 'invert-binary-tree', 'Invert Binary Tree', 'Easy', ['python'], ['tree', 'dfs']],
  [11, 'max-depth-binary-tree', 'Maximum Depth of Binary Tree', 'Easy', ['python'], ['tree', 'dfs']],
  [11, 'level-order-traversal', 'Binary Tree Level Order Traversal', 'Medium', ['python'], ['tree', 'bfs']],
  [11, 'validate-bst', 'Validate Binary Search Tree', 'Medium', ['python'], ['tree', 'dfs']],
  [11, 'subsets', 'Subsets', 'Medium', ['python'], ['backtracking']],
  [11, 'permutations', 'Permutations', 'Medium', ['python'], ['backtracking']],
  [11, 'climbing-stairs', 'Climbing Stairs', 'Easy', ['python'], ['dp']],
  [11, 'coin-change', 'Coin Change', 'Medium', ['python'], ['dp']],
  [11, 'house-robber', 'House Robber', 'Medium', ['python'], ['dp']],
];

const STUB = {
  python: '# Not authored yet.\n# This problem is part of the stack but its starter code,\n# tests, and reference solution have not been written.\n',
  c: '/* Not authored yet. */\n',
  cpp: '// Not authored yet.\n',
};

function main() {
  if (!fs.existsSync(PROBLEMS_DIR)) fs.mkdirSync(PROBLEMS_DIR, { recursive: true });
  let created = 0;
  STACK.forEach(([phase, id, title, difficulty, languages, tags], i) => {
    const stackIndex = i + 1;
    const dir = path.join(PROBLEMS_DIR, id);
    const metaPath = path.join(dir, 'meta.json');
    if (fs.existsSync(metaPath)) return; // never clobber authored problems
    fs.mkdirSync(dir, { recursive: true });
    const meta = {
      title,
      phase,
      phaseName: PHASE_NAMES[phase],
      difficulty,
      tags,
      languages,
      stackIndex,
      authored: false,
    };
    fs.writeFileSync(metaPath, JSON.stringify(meta, null, 2) + '\n');
    fs.writeFileSync(
      path.join(dir, 'description.md'),
      `# ${title}\n\n> **Not authored yet.** This problem is in the stack but hasn't been written up.\n\nPhase ${phase} — ${PHASE_NAMES[phase]}.\n`
    );
    for (const lang of languages) {
      const ext = lang === 'cpp' ? 'cpp' : lang === 'c' ? 'c' : 'py';
      fs.writeFileSync(path.join(dir, `starter.${ext}`), STUB[lang] || STUB.python);
    }
    created++;
  });
  console.log(`Scaffold complete. Created ${created} placeholder problem(s).`);
}

main();
