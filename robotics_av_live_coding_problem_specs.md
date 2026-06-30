# Robotics / AV SWE Live-Coding Practice Problems

Purpose: add these as discrete problems to a LeetCode-style test harness.

Scope: timed/live coding interview prep only. Exclude take-home projects and domain-only rounds. Robotics-specific problems here mostly appear as graph/grid routing, discrete-time simulation, geometry, C++ systems structures, and standard DS&A with robotics/AV wrappers.

Evidence tags:
- `first_person_report`: reported directly by candidates in interview reports.
- `aggregator_reported`: listed by interview-problem aggregators for the named company.
- `repeated_pattern`: recurring category across robotics/AV interview reports.
- `domain_wrapped`: standard DS&A problem commonly framed with robotics/AV language.

Implementation guidance:
- Generate problem statements, examples, hidden tests, and reference solutions.
- Prefer C++17 solutions unless the harness is language-agnostic.
- Each problem should include time and space complexity checks in the editorial.
- Include edge-case tests, not only happy paths.
- Avoid ROS, EKF, PID, kinematics, odometry, and perception pipelines here unless they are framed as short live-coding problems.

---

## Warm Up

These are the 5 easiest problems from the set. Use them as daily 10–15 minute drills.

### W1. Valid Palindrome

```yaml
slug: valid-palindrome-normalized
difficulty: easy
category: string
evidence: first_person_report
priority_reason: directly reported in an Amazon Robotics-style live coding round; low difficulty but useful as a pressure warm-up
```

Implement a function that returns whether a string is a palindrome after ignoring non-alphanumeric characters and case.

Required variants:
- ASCII-only version.
- Optional Unicode-safe version if the harness supports it.

Test focus:
- empty string
- single character
- mixed case
- punctuation and spaces
- numeric characters
- no valid alphanumeric characters

Expected technique:
- two pointers
- O(n) time, O(1) extra space

---

### W2. Moving Average From Data Stream

```yaml
slug: moving-average-from-data-stream
difficulty: easy
category: queue
evidence: repeated_pattern
priority_reason: common live-coding stream-processing primitive; maps to sensor windows
```

Design a class that receives integer samples and returns the moving average over the last `k` samples.

API:
```cpp
class MovingAverage {
public:
    MovingAverage(int k);
    double next(int value);
};
```

Test focus:
- fewer than `k` values
- exactly `k` values
- more than `k` values
- negative values
- large values requiring `int64_t` sum

Expected technique:
- bounded queue/ring buffer
- running sum
- O(1) per sample

---

### W3. Min Stack

```yaml
slug: min-stack
difficulty: easy_medium
category: stack
evidence: repeated_pattern
priority_reason: common live-coding bounded-state structure
```

Implement a stack supporting `push`, `pop`, `top`, and `getMin` in O(1).

Test focus:
- duplicate minimum values
- popping the current minimum
- negative values
- single-element stack
- invalid operations if your harness tests exceptions/errors

Expected technique:
- auxiliary min stack or pair stack
- O(1) per operation

---

### W4. Merge Intervals

```yaml
slug: merge-intervals-maintenance-windows
difficulty: medium
category: intervals
evidence: aggregator_reported
company_context: Zoox-style maintenance windows
priority_reason: frequently reported standard live-coding problem; easy to wrap in robotics scheduling language
```

Given a list of closed intervals `[start, end]`, merge all overlapping intervals.

Robotics/AV wrapper:
- maintenance windows
- sensor blackout windows
- road-closure time windows

Test focus:
- empty input
- single interval
- touching intervals `[1,2]` and `[2,3]`
- nested intervals
- unsorted input
- negative timestamps if supported

Expected technique:
- sort by start
- sweep and merge
- O(n log n) time

---

### W5. Number of Islands

```yaml
slug: number-of-islands-occupancy-components
difficulty: medium
category: grid_bfs_dfs
evidence: repeated_pattern
priority_reason: foundational grid traversal; maps to connected obstacle/free-space components
```

Given a binary grid, count connected components of `1`s using 4-connectivity.

Robotics wrapper:
- count connected obstacle regions in an occupancy grid
- count connected traversable regions

Test focus:
- empty grid
- all water/free cells
- all land/obstacle cells
- diagonal-only adjacency should not connect
- thin corridors
- large grid

Expected technique:
- BFS or DFS flood fill
- O(rows * cols) time

---

# Main Priority List

## 1. Shortest Path in Binary Matrix

```yaml
slug: shortest-path-binary-occupancy-grid
difficulty: medium
category: grid_bfs
evidence: repeated_pattern
priority_reason: highest-frequency robotics/AV-flavored live-coding shape: route planning on a grid with obstacles
```

Given an `n x n` grid where `0` is free and `1` is blocked, return the length of the shortest path from top-left to bottom-right using 8-connected movement. Return `-1` if no path exists.

Robotics wrapper:
- occupancy-grid shortest path
- local planner on a binary costmap
- route around blocked cells

Test focus:
- start blocked
- goal blocked
- `1 x 1` grid
- no path
- diagonal-only path
- dense obstacles
- multiple shortest paths

Expected technique:
- BFS
- visited matrix
- O(n²) time, O(n²) space

---

## 2. Multi-Source Grid Propagation

```yaml
slug: rotting-oranges-wavefront-propagation
difficulty: medium
category: multi_source_bfs
evidence: aggregator_reported
company_context: Skydio-style Rotting Oranges
priority_reason: common live grid problem; maps directly to wavefront expansion / distance fields
```

Given a grid with empty cells, fresh targets, and active sources, return the minimum number of time steps required for all targets to be reached. Return `-1` if impossible.

Robotics wrapper:
- wavefront expansion from multiple hazards
- propagation of obstacle influence
- distance-to-nearest-source field

Canonical version:
- LeetCode Rotting Oranges.

Required variant:
- Also generate a `Walls and Gates`-style version where empty rooms receive distance to nearest gate/source.

Test focus:
- no sources
- no targets
- unreachable target
- multiple sources
- simultaneous propagation
- large rectangular grid

Expected technique:
- enqueue all sources initially
- BFS by layer/time
- O(rows * cols)

---

## 3. Weighted Grid Path Planning

```yaml
slug: weighted-grid-path-dijkstra-costmap
difficulty: medium_hard
category: dijkstra_grid
evidence: repeated_pattern
priority_reason: common escalation from binary BFS to costmap routing; strongest robotics-specific algorithmic family
```

Given a grid of non-negative traversal costs, return the minimum-cost path from start to goal using 4-connected movement.

Required versions:
1. Return only minimum cost.
2. Return actual path as list of coordinates.
3. Add blocked cells represented by `-1`.

Suggested canonical variants:
- Path With Minimum Effort.
- Swim in Rising Water.
- Minimum cost through a costmap.

Test focus:
- start equals goal
- blocked start or goal
- no path
- equal-cost tie cases
- high-cost detour vs short expensive path
- large grid

Expected technique:
- Dijkstra with min-heap
- parent reconstruction for path-returning version
- O(rows * cols * log(rows * cols))

Optional extension:
- Add Manhattan or Euclidean heuristic and implement A*.
- Verify A* returns same optimal cost when heuristic is admissible.

---

## 4. General Graph Shortest Path

```yaml
slug: network-delay-route-graph
difficulty: medium
category: graph_dijkstra
evidence: repeated_pattern
priority_reason: road-network / robot-route framing often reduces to general graph shortest path
```

Given a directed weighted graph and a source node, return the time for all nodes to receive a signal, or `-1` if some node is unreachable.

Robotics wrapper:
- route time across road graph
- command propagation through robot fleet network
- navigation graph cost propagation

Canonical version:
- Network Delay Time.

Test focus:
- disconnected graph
- multiple edges between same nodes
- zero-weight edges if allowed
- one node
- cycle
- large sparse graph

Expected technique:
- adjacency list
- Dijkstra
- O((V + E) log V)

---

## 5. Zoox-Style Lane Crossing Simulation

```yaml
slug: lane-crossing-discrete-time-simulation
difficulty: hard
category: simulation_collision
evidence: first_person_report
company_context: Zoox-style lane crossing / lane change maneuver
priority_reason: highest-signal AV-specific live-coding problem family
```

Implement a discrete-time simulation to determine whether an agent can cross `N` lanes without colliding with moving cars.

Problem model:
- Lanes are indexed `0..N-1`.
- Each lane contains cars with initial position, velocity, and length.
- Time advances in integer ticks.
- At each tick, the agent may wait or move forward by one lane.
- A collision occurs if the agent's crossing position overlaps any car interval in the lane at that tick.
- Return whether a collision-free sequence exists.
- Optionally return one valid sequence of actions.

Inputs to define:
```cpp
struct Car {
    int lane;
    double position;
    double velocity;
    double length;
};

bool canCross(int lane_count, vector<Car> cars, double crossing_x, int max_time);
```

Test focus:
- no cars
- immediate collision in first lane
- must wait before crossing
- impossible due to continuous blockage
- car exactly touching crossing point boundary
- negative velocity
- multiple cars per lane
- max-time cutoff

Expected technique:
- BFS/DP over `(time, lane_index)`
- collision check per lane/time
- precompute unsafe `(lane, time)` cells if simple
- O(max_time * lane_count * cars_per_lane)

Important edge semantics:
- Define whether touching endpoints counts as collision.
- Define whether the agent occupies zero length or has width.
- Define when movement occurs relative to car update: before or after tick.

---

## 6. LRU Cache

```yaml
slug: lru-cache-trajectory-path-cache
difficulty: medium
category: hashmap_linked_list
evidence: aggregator_reported
company_context: Zoox-style trajectory/path cache
priority_reason: common high-frequency live systems-DSA question; easy to robotics-wrap as a planner cache
```

Design an LRU cache with fixed capacity supporting `get(key)` and `put(key, value)` in O(1).

Robotics wrapper:
- trajectory cache
- route cache
- perception result cache

API:
```cpp
class LRUCache {
public:
    LRUCache(int capacity);
    int get(int key);
    void put(int key, int value);
};
```

Test focus:
- update existing key
- capacity 1
- eviction order
- get updates recency
- put updates recency
- repeated operations
- zero capacity if your harness supports invalid inputs

Expected technique:
- unordered_map from key to list iterator
- doubly linked list or custom nodes
- O(1) operations

---

## 7. Dependency Ordering / Topological Sort

```yaml
slug: dependency-order-build-graph
difficulty: medium
category: graph_topological_sort
evidence: first_person_report
priority_reason: recurring live coding pattern in autonomy-adjacent reports; useful for build/component dependency wrappers
```

Given `n` modules and dependency pairs `[a, b]` meaning `a` depends on `b`, return a valid build order. Return empty if impossible.

Robotics wrapper:
- robot software module startup order
- file/package dependency order
- sensor pipeline dependency ordering

Test focus:
- no dependencies
- simple chain
- diamond dependency
- cycle
- disconnected components
- duplicate edges
- self-dependency

Expected technique:
- Kahn's algorithm or DFS postorder
- cycle detection
- O(V + E)

---

## 8. Search Suggestions System

```yaml
slug: search-suggestions-system
difficulty: medium
category: trie_sorting
evidence: aggregator_reported
company_context: Skydio-style reported problem
priority_reason: common large-company live problem; useful prefix-search structure
```

Given a list of product/command names and a typed search word, return up to 3 lexicographically smallest suggestions after each typed character.

Robotics wrapper:
- command autocomplete
- map-location autocomplete
- diagnostic command search

Test focus:
- duplicate words
- fewer than 3 matches
- no matches after prefix
- prefix equal to whole word
- mixed lengths
- lexicographic ordering

Expected techniques:
- sort and binary search by prefix
- or trie with top-k suggestions at each node
- O(n log n + m log n) or trie-based equivalent

---

## 9. Insert Delete GetRandom O(1)

```yaml
slug: randomized-set-o1
difficulty: medium
category: hashmap_array
evidence: aggregator_reported
company_context: Skydio-style reported problem
priority_reason: common implementation-heavy live problem
```

Design a data structure supporting `insert`, `remove`, and `getRandom` in average O(1).

Robotics wrapper:
- random sampling from active landmarks
- randomized test-case selection
- active-object registry

API:
```cpp
class RandomizedSet {
public:
    bool insert(int val);
    bool remove(int val);
    int getRandom();
};
```

Test focus:
- insert duplicate
- remove missing
- remove last element
- remove middle element using swap-delete
- random output belongs to set
- all operations after many mutations

Expected technique:
- vector of values
- unordered_map from value to index
- swap with last on removal
- O(1) average

---

## 10. Sliding Window Maximum

```yaml
slug: sliding-window-maximum-sensor-window
difficulty: hard
category: monotonic_queue
evidence: repeated_pattern
priority_reason: common stream/window problem; maps to sensor-window processing
```

Given an array and window size `k`, return the maximum value in every contiguous window.

Robotics wrapper:
- max obstacle confidence over recent sensor frames
- peak latency over sliding time window
- max current draw in telemetry window

Test focus:
- `k = 1`
- `k = n`
- repeated values
- strictly increasing
- strictly decreasing
- negative values

Expected technique:
- monotonic deque of indices
- O(n) time, O(k) space

---

## 11. K Closest Points to Origin

```yaml
slug: k-closest-points-to-origin
difficulty: medium
category: heap_quickselect_geometry
evidence: repeated_pattern
priority_reason: common heap/geometry problem; maps to nearest obstacle/landmark selection
```

Given points `(x, y)`, return the `k` closest to origin by Euclidean distance.

Robotics wrapper:
- nearest landmarks
- nearest obstacles
- closest detected objects

Test focus:
- `k = 0`
- `k = n`
- duplicate distances
- negative coordinates
- large coordinate values requiring squared-distance overflow handling
- deterministic tie behavior if required

Expected techniques:
- max-heap of size `k`
- or quickselect
- compare squared distances
- O(n log k) heap or average O(n) quickselect

---

## 12. Lowest Common Ancestor of Binary Tree

```yaml
slug: lowest-common-ancestor-component-tree
difficulty: medium
category: binary_tree
evidence: aggregator_reported
company_context: Zoox-style component hierarchy
priority_reason: common tree traversal question with system hierarchy wrapper
```

Given a binary tree and two nodes, return their lowest common ancestor.

Robotics wrapper:
- component hierarchy
- subsystem ownership tree
- transform tree simplified to binary form

Test focus:
- one target is ancestor of other
- targets on different subtrees
- root is LCA
- missing node variant
- single-node tree

Expected technique:
- recursive postorder
- O(n) time

Required variant:
- If either node may be absent, return null unless both exist.

---

## 13. Filesystem Filter / Nested File Search

```yaml
slug: filesystem-filter-nested-search
difficulty: medium
category: tree_dfs_design
evidence: first_person_report
priority_reason: reported in robotics-adjacent interview loops; tests class design and traversal
```

Design and implement a nested filesystem search/filter.

Inputs:
- files have name, size, extension, modified timestamp
- directories contain files and directories
- filters can include extension, min/max size, name substring, modified-before/after

Return all matching files.

Robotics wrapper:
- log file search
- recorded bag/session filtering
- artifact cleanup

Test focus:
- empty directory
- nested directories
- multiple filters combined with AND
- no matches
- duplicate filenames in different directories
- very deep nesting
- invalid filter

Expected technique:
- recursive DFS or iterative stack
- composable predicates
- O(number_of_nodes)

Optional extension:
- Support OR filters.
- Support sorted output by path or timestamp.
- Support early pruning for directory metadata if modeled.

---

## 14. Image Resize / Pixel Mapping

```yaml
slug: image-resize-nearest-bilinear
difficulty: medium
category: matrix_image
evidence: first_person_report
company_context: Brain Corp-style reported question
priority_reason: one of the few explicitly reported robotics-company coding problems with perception flavor
```

Implement image resizing for a grayscale image represented as a 2D matrix.

Required versions:
1. nearest-neighbor resize
2. bilinear resize

Inputs:
```cpp
vector<vector<uint8_t>> image;
int out_height;
int out_width;
```

Test focus:
- same size
- upscale
- downscale
- one-pixel image
- non-square image
- empty image invalid input
- boundary pixels
- interpolation rounding behavior

Expected technique:
- coordinate mapping from output pixel to source coordinate
- clamp boundaries
- nearest-neighbor first
- bilinear interpolation second

---

## 15. C++ Thread-Safe Bounded Queue

```yaml
slug: thread-safe-bounded-queue
difficulty: medium_hard
category: cpp_concurrency
evidence: first_person_report
priority_reason: robotics interviews often include C++ threading, deadlock, queues, mutexes, and producer-consumer structures
```

Implement a bounded blocking queue for producer/consumer use.

API:
```cpp
template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(size_t capacity);
    void push(T value);
    T pop();
    bool try_push(T value);
    optional<T> try_pop();
    size_t size() const;
};
```

Test focus:
- FIFO order
- capacity 1
- multiple producers
- multiple consumers
- push blocks when full
- pop blocks when empty
- try operations do not block
- no data races under ThreadSanitizer if available

Expected technique:
- mutex
- condition_variable
- deque or ring buffer
- correct wakeup predicate
- no busy wait

Optional hard variant:
- Implement non-blocking fixed-capacity ring buffer for single-producer/single-consumer.

---

## 16. Basic Calculator

```yaml
slug: basic-calculator-expression-parser
difficulty: hard
category: stack_parser
evidence: aggregator_reported
company_context: Zoox-style reported problem
priority_reason: confirmed-style large-company live coding breadth; lower robotics specificity
```

Implement an expression evaluator.

Required versions:
1. Basic Calculator II: `+`, `-`, `*`, `/`, non-negative integers, spaces.
2. Optional Basic Calculator I: `+`, `-`, parentheses.

Test focus:
- whitespace
- multi-digit numbers
- operator precedence
- integer division truncation semantics
- leading negative if supported
- nested parentheses for version I
- malformed input if your harness supports invalid tests

Expected technique:
- stack for terms
- sign tracking
- recursive descent or stack for parentheses

---

## 17. Edit Distance

```yaml
slug: edit-distance
difficulty: hard
category: dynamic_programming
evidence: aggregator_reported
company_context: Zoox-style reported problem
priority_reason: common hard DP in live coding; low robotics specificity
```

Given two strings, return the minimum number of insertions, deletions, and substitutions required to convert one into the other.

Robotics wrapper:
- compare command strings
- align symbolic paths
- compare planned route token sequences

Test focus:
- empty strings
- identical strings
- one insertion
- one deletion
- one substitution
- completely different strings
- long strings for memory optimization

Expected technique:
- 2D DP
- O(mn) time
- O(min(m,n)) space optimized variant

---

## 18. Container With Most Water

```yaml
slug: container-with-most-water
difficulty: medium
category: two_pointers
evidence: first_person_report
company_context: Waymo-style reported problem
priority_reason: company-specific reported live coding problem; generic but worth a timed drill
```

Given an array of heights, return the maximum water area formed by two vertical lines.

Test focus:
- two elements
- all equal heights
- strictly increasing
- strictly decreasing
- large values
- repeated maxima

Expected technique:
- two pointers
- move pointer with smaller height
- O(n) time, O(1) space

---

## 19. Polygon Area

```yaml
slug: polygon-area-shoelace
difficulty: medium
category: geometry
evidence: aggregator_reported
company_context: Zoox-style area of shape
priority_reason: light geometry appears in AV interviews; simple but easy to fumble
```

Given vertices of a simple polygon in order, compute its area.

Test focus:
- triangle
- rectangle
- concave polygon
- clockwise vertices
- counterclockwise vertices
- repeated first/last vertex
- large coordinates

Expected technique:
- shoelace formula
- absolute value / 2
- use 64-bit or double accumulation

---

## 20. Distance From Point to Line in 3D

```yaml
slug: point-to-line-distance-3d
difficulty: medium
category: vector_geometry
evidence: aggregator_reported
company_context: Zoox-style geometry report
priority_reason: AV interviews often include light vector geometry
```

Given point `P` and a 3D line defined by points `A` and `B`, return the shortest distance from `P` to the infinite line.

Required variant:
- Also implement distance to finite segment `AB`.

Test focus:
- point on line
- vertical/horizontal arbitrary 3D line
- degenerate line where `A == B`
- closest point before `A`
- closest point after `B`
- large/small floating-point values

Expected technique:
- infinite line: `norm(cross(P-A, B-A)) / norm(B-A)`
- segment: projection parameter clamped to `[0,1]`
- double precision

---

## 21. Swept AABB Collision

```yaml
slug: swept-aabb-collision-time
difficulty: hard
category: geometry_collision
evidence: repeated_pattern
company_context: Waymo-style two moving bounding boxes
priority_reason: high-signal AV simulation/collision live problem
```

Given two axis-aligned bounding boxes with positions, sizes, and velocities, determine whether they collide within time horizon `T`. Return collision time if one exists.

Inputs:
```cpp
struct AABB {
    double min_x, min_y;
    double max_x, max_y;
    double vx, vy;
};
```

Test focus:
- already overlapping at `t=0`
- never collide
- collide exactly at `T`
- touching boundary semantics
- same velocity
- moving apart
- one stationary, one moving
- zero relative velocity on one axis

Expected technique:
- relative velocity
- per-axis entry and exit times
- global entry = max(axis entries)
- global exit = min(axis exits)
- collision if `entry <= exit` and interval intersects `[0,T]`

Important edge semantics:
- Define whether touching counts as collision.
- Handle zero relative velocity explicitly.

---

## 22. Point In Field of View

```yaml
slug: point-in-field-of-view
difficulty: medium
category: geometry
evidence: repeated_pattern
company_context: Waymo-style shape visible within FOV
priority_reason: common AV geometry wrapper; good vector/math live drill
```

Given observer position, heading vector, field-of-view angle, max range, and target point, determine whether the target is visible.

Required variants:
1. point target
2. circular target with radius
3. list of points, return visible subset

Test focus:
- target at observer position
- target exactly on angle boundary
- target exactly at max range
- target behind observer
- non-normalized heading
- zero heading vector invalid input

Expected technique:
- vector from observer to target
- distance check
- dot product angle check using cosine threshold
- avoid `acos` if possible

---

## 23. Top K Frequent Words

```yaml
slug: top-k-frequent-words
difficulty: medium
category: hashmap_heap_sorting
evidence: aggregator_reported
company_context: Zoox-style reported problem
priority_reason: common live hashmap/heap problem; useful for runtime tradeoff discussion
```

Given a list of words, return the `k` most frequent words. Sort ties lexicographically ascending.

Robotics wrapper:
- top diagnostic messages
- most frequent log tokens
- most common object labels

Test focus:
- ties
- `k = 1`
- `k = unique_count`
- duplicate words
- case sensitivity if defined
- empty input invalid or returns empty

Expected techniques:
- hashmap count
- heap with custom comparator
- or sort unique words
- O(n log k) or O(n log n)

---

## 24. Recursive Function Trace / Iterative Rewrite

```yaml
slug: recursive-trace-iterative-rewrite
difficulty: medium
category: recursion_stack
evidence: first_person_report
priority_reason: reported in small robotics-company interviews; tests execution tracing and stack reasoning
```

Generate a problem where the candidate is given a recursive function over an integer or linked structure, must:
1. determine the output for a concrete input
2. rewrite it iteratively
3. state time and stack complexity

Suggested concrete version:
- recursive linked-list length
- recursive reverse print
- recursive tree sum
- recursive Fibonacci with memoization follow-up

Test focus:
- base case
- empty structure
- single element
- deep recursion stack overflow concern
- iterative equivalence

Expected technique:
- explicit stack or loop
- understand call stack behavior

---

## 25. Linked Structure Cycle / Longest Valid Chain

```yaml
slug: linked-structure-cycle-valid-chain
difficulty: medium
category: linked_list_graph_cycle
evidence: first_person_report
priority_reason: robotics-company report included cyclic linked-structure style coding
```

Given nodes where each node points to at most one next node, find the length of the longest chain that does not repeat a node. Detect cycles.

Robotics wrapper:
- component dependency chain
- route predecessor chain
- state transition chain

Test focus:
- no cycle
- simple cycle
- tail into cycle
- disconnected components
- self-cycle
- null next pointer
- all nodes isolated

Expected techniques:
- visited states: unvisited / visiting / done
- or Floyd cycle detection for linked-list-only variant
- O(n) time

---

# Problems intentionally excluded from live-coding harness

Do not add these as primary LeetCode-style problems unless creating a separate domain-round module:

- ROS publisher/subscriber node
- ROS launch/config exercises
- wheel-encoder odometry
- PID controller
- EKF / Kalman filter implementation
- forward/inverse kinematics
- SE(3) transform compose/invert
- quaternion math
- camera projection
- costmap inflation as full robotics pipeline
- SLAM / scan matching
- point-cloud registration

Reason: these are useful robotics interview knowledge areas, but reports place them more often in take-home, whiteboard, math, system-design, or domain-debugging rounds rather than timed live-coding rounds.

---

# Recommended generation order for the coding agent

1. Generate Warm Up first.
2. Generate Main Priority List problems 1–12 next.
3. Generate geometry/simulation problems 19–22.
4. Generate remaining breadth problems 13–18 and 23–25.
5. For each problem, generate:
   - title
   - slug
   - difficulty
   - tags
   - problem statement
   - examples
   - constraints
   - visible tests
   - hidden tests
   - reference solution
   - complexity analysis
   - common failure modes

# Common failure modes to encode as tests

- off-by-one grid bounds
- start/goal blocked
- diagonal vs 4-connected movement confusion
- stale visited-state bugs
- integer overflow in geometry/distance
- duplicate keys/values
- tie-breaking not deterministic
- boundary-touch collision semantics
- zero velocity / zero-length vector
- empty input
- single-element input
- no-solution cases
