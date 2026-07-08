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

static std::string fmtVec(const std::vector<int>& v) {
    std::string s = "[";
    for (int i = 0; i < (int)v.size(); i++) { if (i) s += ", "; s += std::to_string(v[i]); }
    return s + "]";
}

static void check(const char* name, std::vector<int> got,
                  std::vector<int> expected, const char* input) {
    std::string gs = fmtVec(got), es = fmtVec(expected);
    nlc_case(name, got == expected, input, es.c_str(), gs.c_str(), "");
}

int main() {
    nlc_begin();

    // Test 1: k=1 → same as input
    {
        std::vector<int> nums = {3,1,4,1,5};
        check("k=1 same as input", maxSlidingWindow(nums, 1), {3,1,4,1,5}, "[3,1,4,1,5] k=1");
    }

    // Test 2: k=n → single element (global max)
    {
        std::vector<int> nums = {3,1,4,1,5,9};
        check("k=n global max", maxSlidingWindow(nums, 6), {9}, "[3,1,4,1,5,9] k=6");
    }

    // Test 3: strictly increasing
    {
        std::vector<int> nums = {1,2,3,4,5};
        check("strictly increasing k=3", maxSlidingWindow(nums, 3), {3,4,5}, "[1,2,3,4,5] k=3");
    }

    // Test 4: strictly decreasing
    {
        std::vector<int> nums = {5,4,3,2,1};
        check("strictly decreasing k=3", maxSlidingWindow(nums, 3), {5,4,3}, "[5,4,3,2,1] k=3");
    }

    // Test 5: repeated values
    {
        std::vector<int> nums = {3,3,3,3,3};
        check("repeated values", maxSlidingWindow(nums, 3), {3,3,3}, "[3,3,3,3,3] k=3");
    }

    // Test 6: negative values
    {
        std::vector<int> nums = {-5,-3,-1,-4,-2};
        check("negative values k=3", maxSlidingWindow(nums, 3), {-1,-1,-1}, "[-5,-3,-1,-4,-2] k=3");
    }

    // Test 7: mixed positive/negative (classic example)
    {
        std::vector<int> nums = {1,3,-1,-3,5,3,6,7};
        check("mixed classic example", maxSlidingWindow(nums, 3), {3,3,5,5,6,7}, "[1,3,-1,-3,5,3,6,7] k=3");
    }

    nlc_end();
    return 0;
}
