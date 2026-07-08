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

static std::string fmtIntervals(const std::vector<std::vector<int>>& v) {
    std::string s = "[";
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) s += ",";
        s += "[" + std::to_string(v[i][0]) + "," + std::to_string(v[i][1]) + "]";
    }
    s += "]";
    return s;
}

static void check(const char* name, std::vector<std::vector<int>> got,
                  std::vector<std::vector<int>> expected, const char* input) {
    std::string gs = fmtIntervals(got);
    std::string es = fmtIntervals(expected);
    nlc_case(name, got == expected, input, es.c_str(), gs.c_str(), "");
}

int main() {
    nlc_begin();

    // Test 1: empty input
    check("empty input", mergeIntervals({}), {}, "[]");

    // Test 2: single interval
    check("single interval", mergeIntervals({{1,3}}), {{1,3}}, "[[1,3]]");

    // Test 3: two non-overlapping
    check("two non-overlapping", mergeIntervals({{1,2},{4,5}}), {{1,2},{4,5}}, "[[1,2],[4,5]]");

    // Test 4: two overlapping
    check("two overlapping", mergeIntervals({{1,4},{2,6}}), {{1,6}}, "[[1,4],[2,6]]");

    // Test 5: touching intervals [1,2] [2,3]
    check("touching intervals", mergeIntervals({{1,2},{2,3}}), {{1,3}}, "[[1,2],[2,3]]");

    // Test 6: nested (one fully inside another)
    check("nested intervals", mergeIntervals({{1,10},{2,5}}), {{1,10}}, "[[1,10],[2,5]]");

    // Test 7: unsorted input
    check("unsorted input", mergeIntervals({{8,10},{1,3},{2,6},{15,18}}),
          {{1,6},{8,10},{15,18}}, "[[8,10],[1,3],[2,6],[15,18]]");

    // Test 8: all merge into one
    check("all merge into one", mergeIntervals({{1,5},{2,6},{3,7},{4,9}}),
          {{1,9}}, "[[1,5],[2,6],[3,7],[4,9]]");

    nlc_end();
    return 0;
}
