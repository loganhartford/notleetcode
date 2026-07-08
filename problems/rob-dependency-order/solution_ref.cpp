#include <vector>
#include <queue>
#include <set>

std::vector<int> buildOrder(int n, std::vector<std::vector<int>> deps) {
    std::vector<int> indegree(n, 0);
    // adj[b] contains a: b must come before a
    std::vector<std::set<int>> adj(n);

    for (auto& dep : deps) {
        int a = dep[0], b = dep[1];
        if (a == b) return {}; // self-loop
        if (adj[b].insert(a).second) {
            indegree[a]++;
        }
    }

    std::queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indegree[i] == 0) q.push(i);
    }

    std::vector<int> result;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        result.push_back(node);
        for (int next : adj[node]) {
            if (--indegree[next] == 0) q.push(next);
        }
    }

    if ((int)result.size() != n) return {}; // cycle
    return result;
}
