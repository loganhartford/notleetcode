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

// Validate a topological order: correct size + all deps satisfied
static bool isValidOrder(const std::vector<int>& order, int n,
                         const std::vector<std::vector<int>>& deps) {
    if ((int)order.size() != n) return false;
    std::vector<int> pos(n);
    for (int i = 0; i < n; i++) pos[order[i]] = i;
    for (auto& dep : deps) {
        int a = dep[0], b = dep[1];
        if (pos[b] >= pos[a]) return false; // b must come before a
    }
    return true;
}

static void checkValid(const char* name, std::vector<int> got, int n,
                       std::vector<std::vector<int>> deps, const char* input) {
    bool ok = isValidOrder(got, n, deps);
    std::string gs;
    if (got.empty()) gs = "[]";
    else { gs = "["; for (int i=0;i<(int)got.size();i++){if(i)gs+=","; gs+=std::to_string(got[i]);} gs+="]"; }
    nlc_case(name, ok ? 1 : 0, input, "valid topological order", gs.c_str(), "");
}

static void checkEmpty(const char* name, std::vector<int> got, const char* input) {
    nlc_case(name, got.empty() ? 1 : 0, input, "[] (cycle detected)",
             got.empty() ? "[]" : "non-empty", "");
}

int main() {
    nlc_begin();

    // Test 1: no dependencies → any permutation valid
    checkValid("no dependencies n=3", buildOrder(3, {}), 3, {}, "n=3, deps=[]");

    // Test 2: simple chain 0→1→2 (0 must build before 1, 1 before 2)
    // deps: [1,0] means 1 depends on 0, [2,1] means 2 depends on 1
    {
        std::vector<std::vector<int>> deps = {{1,0},{2,1}};
        checkValid("simple chain", buildOrder(3, deps), 3, deps, "0→1→2 chain");
    }

    // Test 3: diamond dependency
    {
        std::vector<std::vector<int>> deps = {{1,0},{2,0},{3,1},{3,2}};
        checkValid("diamond dependency", buildOrder(4, deps), 4, deps, "diamond 0→1,2→3");
    }

    // Test 4: cycle → empty
    {
        checkEmpty("cycle 0→1→0", buildOrder(2, {{0,1},{1,0}}), "cycle [0,1],[1,0]");
    }

    // Test 5: disconnected components
    {
        std::vector<std::vector<int>> deps = {{1,0}};
        // n=4 but only 0→1 edge; 2 and 3 are independent
        checkValid("disconnected components", buildOrder(4, deps), 4, deps, "n=4, only dep [1,0]");
    }

    // Test 6: duplicate edges (handle gracefully)
    {
        std::vector<std::vector<int>> deps = {{1,0},{1,0},{2,1}};
        checkValid("duplicate edges", buildOrder(3, deps), 3, {{1,0},{2,1}}, "dup edge [1,0]");
    }

    // Test 7: self-dependency [0,0] → cycle
    {
        checkEmpty("self-dependency cycle", buildOrder(2, {{0,0}}), "self-loop [0,0]");
    }

    // Test 8: large n with no deps → valid permutation of 0..n-1
    {
        auto order = buildOrder(10, {});
        std::vector<int> sorted_order = order;
        std::sort(sorted_order.begin(), sorted_order.end());
        std::vector<int> expected(10);
        std::iota(expected.begin(), expected.end(), 0);
        bool ok = (sorted_order == expected);
        nlc_case("large n no deps", ok, "n=10, deps=[]",
                 "all 10 modules present", ok ? "valid" : "invalid", "");
    }

    nlc_end();
    return 0;
}
