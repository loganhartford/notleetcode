#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "nlc.h"
#include "solution.cpp"

static Image makeImage(const std::vector<std::vector<int>>& g) {
    int rows = (int)g.size();
    int cols = rows ? (int)g[0].size() : 0;
    Image img(rows, cols, 0);
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) img.SetAt(r, c, g[r][c]);
    return img;
}

static std::string gridStr(const std::vector<std::vector<int>>& g) {
    if (g.empty()) return "[] (0x0)";
    std::string s;
    for (size_t r = 0; r < g.size(); ++r) {
        if (r) s += "/";
        for (size_t c = 0; c < g[r].size(); ++c) s += (char)('0' + g[r][c]);
    }
    return s;
}

static void check(const char* name, Image img, int expected, const std::string& input) {
    std::string es = std::to_string(expected);
    try {
        int got = CountConnectedPixels(img);
        nlc_case(name, got == expected, input.c_str(), es.c_str(),
                 std::to_string(got).c_str(), "");
    } catch (const std::exception& e) {
        std::string gs = std::string("exception: ") + e.what();
        nlc_case(name, 0, input.c_str(), es.c_str(), gs.c_str(), "");
    } catch (...) {
        nlc_case(name, 0, input.c_str(), es.c_str(), "unknown exception", "");
    }
}

static void checkGrid(const char* name, const std::vector<std::vector<int>>& g, int expected) {
    check(name, makeImage(g), expected, gridStr(g));
}

int main() {
    nlc_begin();

    // Test 1: empty image
    checkGrid("empty image", {}, 0);

    // Test 2: single pixel, off
    checkGrid("1x1 off", {{0}}, 0);

    // Test 3: single pixel, on
    checkGrid("1x1 on", {{1}}, 1);

    // Test 4: nothing on
    checkGrid("3x3 all off", {{0,0,0},{0,0,0},{0,0,0}}, 0);

    // Test 5: everything on
    checkGrid("3x3 all on", {{1,1,1},{1,1,1},{1,1,1}}, 9);

    // Test 6: two blobs, larger one wins
    checkGrid("two blobs", {{1,1,0,0,1},
                            {1,0,0,1,1},
                            {0,0,0,1,0}}, 4);

    // Test 7: diagonals are not connected
    checkGrid("diagonal not connected", {{1,0,0},
                                         {0,1,0},
                                         {0,0,1}}, 1);

    // Test 8: non-square image (catches row/col swaps)
    checkGrid("2x5 non-square", {{1,1,1,0,1},
                                 {0,0,1,0,1}}, 4);

    // Test 9: ring around an isolated center pixel
    checkGrid("ring with hollow center", {{1,1,1,1,1},
                                          {1,0,0,0,1},
                                          {1,0,1,0,1},
                                          {1,0,0,0,1},
                                          {1,1,1,1,1}}, 16);

    // Test 10: checkerboard — every on-pixel is its own component
    {
        std::vector<std::vector<int>> g(5, std::vector<int>(5, 0));
        for (int r = 0; r < 5; ++r)
            for (int c = 0; c < 5; ++c) g[r][c] = ((r + c) % 2 == 0);
        checkGrid("5x5 checkerboard", g, 1);
    }

    // Test 11: 60x40 vertical stripes — 20 components of 60 pixels each
    {
        Image img(60, 40, 0);
        for (int r = 0; r < 60; ++r)
            for (int c = 0; c < 40; c += 2) img.SetAt(r, c, 1);
        check("60x40 vertical stripes", img, 60, "60x40, on where col is even");
    }

    // Test 12: single large component (no revisiting, no stack blowups)
    {
        Image img(120, 120, 1);
        check("120x120 all on", img, 14400, "120x120, every pixel on");
    }

    nlc_end();
    return 0;
}
