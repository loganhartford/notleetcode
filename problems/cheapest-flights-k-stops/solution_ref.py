from typing import List


def findCheapestPrice(n: int, flights: List[List[int]], src: int, dst: int, k: int) -> int:
    INF = float("inf")
    dist = [INF] * n
    dist[src] = 0
    # Bellman-Ford: at most k+1 edges, so k+1 relaxation rounds.
    for _ in range(k + 1):
        tmp = dist[:]
        for u, v, w in flights:
            if dist[u] != INF and dist[u] + w < tmp[v]:
                tmp[v] = dist[u] + w
        dist = tmp
    return -1 if dist[dst] == INF else dist[dst]
