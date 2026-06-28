# Time: O(V + E)  — DFS visits every node and edge once
from typing import Optional


class Node:
    def __init__(self, val=0, neighbors=None):
        self.val = val
        self.neighbors = neighbors if neighbors is not None else []


def cloneGraph(node: Optional["Node"]) -> Optional["Node"]:
    if node is None:
        return None
    clones = {}

    def dfs(n: "Node") -> "Node":
        if n in clones:
            return clones[n]
        copy = Node(n.val)
        clones[n] = copy
        for nb in n.neighbors:
            copy.neighbors.append(dfs(nb))
        return copy

    return dfs(node)
