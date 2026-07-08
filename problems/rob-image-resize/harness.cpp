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

static std::string fmtImg(const std::vector<std::vector<uint8_t>>& img) {
    std::string s = "[";
    for (int i = 0; i < (int)img.size(); i++) {
        if (i) s += ",";
        s += "[";
        for (int j = 0; j < (int)img[i].size(); j++) {
            if (j) s += ",";
            s += std::to_string(img[i][j]);
        }
        s += "]";
    }
    return s + "]";
}

static void check(const char* name,
                  std::vector<std::vector<uint8_t>> got,
                  std::vector<std::vector<uint8_t>> expected,
                  const char* input) {
    std::string gs = fmtImg(got), es = fmtImg(expected);
    nlc_case(name, got == expected, input, es.c_str(), gs.c_str(), "");
}

int main() {
    nlc_begin();

    // Test 1: same size → identical output
    {
        std::vector<std::vector<uint8_t>> img = {{1,2},{3,4}};
        check("same size identity", resizeNearest(img,2,2), img, "2x2→2x2");
    }

    // Test 2: 1x1 single pixel upscale
    {
        std::vector<std::vector<uint8_t>> img = {{128}};
        std::vector<std::vector<uint8_t>> exp = {{128,128},{128,128}};
        check("1x1 to 2x2", resizeNearest(img,2,2), exp, "1x1→2x2 val=128");
    }

    // Test 3: 2x upscale of 2x2
    // src_r for out r=0: (0.5)*2/4=0.25 → 0; r=1: (1.5)*2/4=0.75 → 0; r=2: (2.5)*2/4=1.25→1; r=3: (3.5)*2/4=1.75→1
    // src_c same pattern
    {
        std::vector<std::vector<uint8_t>> img = {{10,20},{30,40}};
        std::vector<std::vector<uint8_t>> exp = {
            {10,10,20,20},
            {10,10,20,20},
            {30,30,40,40},
            {30,30,40,40}
        };
        check("2x2 to 4x4 upscale", resizeNearest(img,4,4), exp, "2x2→4x4");
    }

    // Test 4: downscale 4x4 to 2x2
    // out r=0: (0.5)*4/2=1.0→1; r=1: (1.5)*4/2=3.0→3
    // out c=0: 1, c=1: 3
    {
        std::vector<std::vector<uint8_t>> img = {
            {1,2,3,4},
            {5,6,7,8},
            {9,10,11,12},
            {13,14,15,16}
        };
        std::vector<std::vector<uint8_t>> exp = {{6,8},{14,16}};
        check("4x4 to 2x2 downscale", resizeNearest(img,2,2), exp, "4x4→2x2");
    }

    // Test 5: non-square image (2x3 → 4x6)
    {
        std::vector<std::vector<uint8_t>> img = {{10,20,30},{40,50,60}};
        // src_r: r=0→0, r=1→0, r=2→1, r=3→1 (since 0.5*2/4=0.25→0, 1.5*2/4=0.75→0, 2.5*2/4=1.25→1, 3.5*2/4=1.75→1)
        // src_c: c=0→0, c=1→0, c=2→1, c=3→1, c=4→2, c=5→2
        std::vector<std::vector<uint8_t>> exp = {
            {10,10,20,20,30,30},
            {10,10,20,20,30,30},
            {40,40,50,50,60,60},
            {40,40,50,50,60,60}
        };
        check("2x3 to 4x6 upscale", resizeNearest(img,4,6), exp, "2x3→4x6");
    }

    // Test 6: 1-pixel image any output size → all same value
    {
        std::vector<std::vector<uint8_t>> img = {{200}};
        auto got = resizeNearest(img, 3, 5);
        bool ok = true;
        for (auto& row : got) for (uint8_t v : row) if (v != 200) ok = false;
        bool szOk = got.size()==3 && got[0].size()==5;
        nlc_case("1x1 to 3x5 all same", ok&&szOk, "1x1→3x5", "all 200", ok&&szOk?"ok":"wrong", "");
    }

    // Test 7: wide image resize (1x4 → 1x2)
    // src_c: c=0:(0.5)*4/2=1→1, c=1:(1.5)*4/2=3→3
    {
        std::vector<std::vector<uint8_t>> img = {{1,2,3,4}};
        std::vector<std::vector<uint8_t>> exp = {{2,4}};
        check("1x4 to 1x2 downscale", resizeNearest(img,1,2), exp, "1x4→1x2");
    }

    nlc_end();
    return 0;
}
