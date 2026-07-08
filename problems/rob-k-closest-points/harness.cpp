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

static std::string fmtPts(std::vector<std::vector<int>> pts) {
    std::sort(pts.begin(), pts.end());
    std::string s = "[";
    for (int i = 0; i < (int)pts.size(); i++) {
        if (i) s += ",";
        s += "[" + std::to_string(pts[i][0]) + "," + std::to_string(pts[i][1]) + "]";
    }
    return s + "]";
}

static void check(const char* name, std::vector<std::vector<int>> got,
                  std::vector<std::vector<int>> expected, const char* input) {
    // Sort both for comparison (order doesn't matter)
    std::sort(got.begin(), got.end());
    std::sort(expected.begin(), expected.end());
    std::string gs = fmtPts(got), es = fmtPts(expected);
    nlc_case(name, got == expected, input, es.c_str(), gs.c_str(), "");
}

// For tie cases just check size and that all points have correct distance
static void checkByDist(const char* name, std::vector<std::vector<int>> got,
                        int expectedDist2, int k, const char* input) {
    bool ok = (int)got.size() == k;
    if (ok) {
        for (auto& p : got) {
            if (p[0]*p[0]+p[1]*p[1] != expectedDist2) { ok = false; break; }
        }
    }
    nlc_case(name, ok, input,
             (std::to_string(k) + " points at dist²=" + std::to_string(expectedDist2)).c_str(),
             ok?"correct":"wrong", "");
}

int main() {
    nlc_begin();

    // Test 1: k=1
    check("k=1", kClosest({{1,3},{-2,2}}, 1), {{-2,2}}, "[[1,3],[-2,2]] k=1");

    // Test 2: k=n (all points)
    check("k=n all points", kClosest({{1,0},{0,1}}, 2), {{1,0},{0,1}}, "[[1,0],[0,1]] k=2");

    // Test 3: duplicate distances (valid selection)
    checkByDist("duplicate distances k=2", kClosest({{1,0},{0,1},{-1,0}}, 2), 1, 2,
                "3 points at dist²=1, pick 2");

    // Test 4: negative coordinates
    check("negative coords", kClosest({{-5,-3},{1,1},{2,2}}, 1), {{1,1}}, "negative coords k=1");

    // Test 5: points on axes
    check("points on axes", kClosest({{5,0},{0,3},{0,0}}, 2), {{0,0},{0,3}},
          "[[5,0],[0,3],[0,0]] k=2");

    // Test 6: large coordinates (use squared dist)
    check("large coordinates", kClosest({{10000,10000},{1,1}}, 1), {{1,1}},
          "large vs small k=1");

    // Test 7: k=0 → empty
    {
        auto got = kClosest({{1,2},{3,4}}, 0);
        nlc_case("k=0 empty result", got.empty(), "k=0", "[]",
                 got.empty()?"[]":"non-empty", "");
    }

    nlc_end();
    return 0;
}
