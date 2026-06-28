# Time: O(V + E)  — Kahn's topological sort
from typing import List
from collections import deque


def canFinish(numCourses: int, prerequisites: List[List[int]]) -> bool:
    graph = [[] for _ in range(numCourses)]
    indegree = [0] * numCourses
    for a, b in prerequisites:
        graph[b].append(a)
        indegree[a] += 1

    # Kahn's topological sort: if we can process every node, there's no cycle.
    q = deque(i for i in range(numCourses) if indegree[i] == 0)
    seen = 0
    while q:
        node = q.popleft()
        seen += 1
        for nxt in graph[node]:
            indegree[nxt] -= 1
            if indegree[nxt] == 0:
                q.append(nxt)
    return seen == numCourses
