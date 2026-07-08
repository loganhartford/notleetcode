#include <vector>
#include <queue>

int shortestPath(std::vector<std::vector<int>>& grid) {
    int n = grid.size(), m = grid[0].size();
    if (grid[0][0] == 1 || grid[n-1][m-1] == 1) return -1;
    if (n == 1 && m == 1) return 1;

    std::queue<std::pair<int,int>> q;
    q.push({0,0});
    grid[0][0] = 1; // mark visited
    int dist = 1;
    int dx[] = {-1,-1,-1,0,0,1,1,1};
    int dy[] = {-1,0,1,-1,1,-1,0,1};

    while (!q.empty()) {
        int sz = q.size();
        dist++;
        while (sz--) {
            auto [r,c] = q.front(); q.pop();
            for (int d = 0; d < 8; d++) {
                int nr = r+dx[d], nc = c+dy[d];
                if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
                if (grid[nr][nc] == 1) continue;
                if (nr == n-1 && nc == m-1) return dist;
                grid[nr][nc] = 1;
                q.push({nr,nc});
            }
        }
    }
    return -1;
}
