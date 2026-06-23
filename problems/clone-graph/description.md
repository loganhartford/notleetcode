# Clone Graph

Given a reference to a node in a **connected** undirected graph, return a **deep copy** (clone) of the graph.

Each node contains a value (`int`) and a list of its neighbors:
```python
class Node:
    def __init__(self, val=0, neighbors=None):
        self.val = val
        self.neighbors = neighbors or []
```

The graph is given as an **adjacency list** (1-indexed): the `i`-th list holds the values of the neighbors of node `i+1`. Node values are unique and equal to their 1-based index.

### Example 1
```
Input:  adjList = [[2,4],[1,3],[2,4],[1,3]]
Output: a clone of the same 4-node graph
```
Node 1's neighbors are 2 and 4; node 2's are 1 and 3; etc.

### Example 2
```
Input:  adjList = [[]]       # one node, no neighbors
Output: a clone with a single node, value 1
```

### Example 3
```
Input:  adjList = []         # empty graph
Output: None
```

### Constraints
- `0 <= number of nodes <= 100`
- `1 <= Node.val <= 100`, unique.
- The graph is connected and undirected; no self-loops or repeated edges.

The checker verifies the clone has the **same structure** and that **no node is
shared** with the original (a true deep copy).
