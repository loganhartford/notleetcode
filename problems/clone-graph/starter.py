from typing import Optional


# Definition for a Node (provided by the harness):
class Node:
    def __init__(self, val=0, neighbors=None):
        self.val = val
        self.neighbors = neighbors if neighbors is not None else []


def cloneGraph(node: Optional["Node"]) -> Optional["Node"]:
    pass
