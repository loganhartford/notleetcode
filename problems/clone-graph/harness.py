from collections import deque
from solution import cloneGraph, Node
from nlc import run_cases


def build(adj):
    if not adj:
        return None
    nodes = {i + 1: Node(i + 1) for i in range(len(adj))}
    for i in range(len(adj)):
        nodes[i + 1].neighbors = [nodes[v] for v in adj[i]]
    return nodes[1]


def serialize(root):
    """Return (adjacency dict val->sorted neighbor vals, set of object ids)."""
    adjacency, ids = {}, set()
    if root is None:
        return adjacency, ids
    seen = {id(root)}
    q = deque([root])
    while q:
        cur = q.popleft()
        ids.add(id(cur))
        adjacency[cur.val] = sorted(nb.val for nb in cur.neighbors)
        for nb in cur.neighbors:
            if id(nb) not in seen:
                seen.add(id(nb))
                q.append(nb)
    return adjacency, ids


def driver(adj):
    original = build(adj)
    _, original_ids = serialize(original)
    clone = cloneGraph(original)
    clone_adj, clone_ids = serialize(clone)
    deep = clone_ids.isdisjoint(original_ids)  # no shared nodes
    items = sorted((k, v) for k, v in clone_adj.items())
    return [items, deep]


def expected_for(adj):
    items = sorted((i + 1, sorted(adj[i])) for i in range(len(adj)))
    return [items, True]


ADJ = {
    "four-node ring": [[2, 4], [1, 3], [2, 4], [1, 3]],
    "single node": [[]],
    "empty graph": [],
    "two nodes": [[2], [1]],
    "triangle": [[2, 3], [1, 3], [1, 2]],
}

CASES = [{"name": name, "args": [adj], "expected": expected_for(adj)} for name, adj in ADJ.items()]

run_cases(driver, CASES, fmt_input=lambda c: "adjList={}".format(c["args"][0]))
