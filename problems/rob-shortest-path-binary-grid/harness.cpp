#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "nlc.h"
#include "solution.cpp"

static void check(const char* name, int got, int expected, const char* input) {
    std::string gs = std::to_string(got);
    std::string es = std::to_string(expected);
    nlc_case(name, got == expected, input, es.c_str(), gs.c_str(), "");
}

int main() {
    nlc_begin();

    // Test 1: 1x1 free grid → 1
    {
        std::vector<std::vector<int>> g = {{0}};
        check("1x1 free grid", shortestPath(g), 1, "[[0]]");
    }

    // Test 2: 1x1 blocked → -1
    {
        std::vector<std::vector<int>> g = {{1}};
        check("1x1 blocked", shortestPath(g), -1, "[[1]]");
    }

    // Test 3: start blocked → -1
    {
        std::vector<std::vector<int>> g = {{1,0,0},{0,0,0},{0,0,0}};
        check("start blocked", shortestPath(g), -1, "start=1");
    }

    // Test 4: goal blocked → -1
    {
        std::vector<std::vector<int>> g = {{0,0,0},{0,0,0},{0,0,1}};
        check("goal blocked", shortestPath(g), -1, "goal=1");
    }

    // Test 5: no path (blocked wall)
    {
        std::vector<std::vector<int>> g = {{0,0,0},{1,1,1},{0,0,0}};
        check("no path blocked wall", shortestPath(g), -1, "middle row blocked");
    }

    // Test 6: diagonal-only path
    {
        std::vector<std::vector<int>> g = {{0,1},{1,0}};
        check("diagonal only path", shortestPath(g), 2, "[[0,1],[1,0]]");
    }

    // Test 7: 3x3 clear grid → 3 (diagonal)
    {
        std::vector<std::vector<int>> g = {{0,0,0},{0,0,0},{0,0,0}};
        check("3x3 clear grid", shortestPath(g), 3, "3x3 all zeros");
    }

    // Test 8: dense obstacles with path
    {
        std::vector<std::vector<int>> g = {
            {0,1,0,0},
            {0,1,0,1},
            {0,0,0,0}
        };
        check("dense obstacles with path", shortestPath(g), 5, "3x4 with obstacles");
    }

    nlc_end();
    return 0;
}
