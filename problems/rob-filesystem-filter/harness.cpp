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

static std::string fmtPaths(std::vector<std::string> v) {
    std::sort(v.begin(), v.end());
    std::string s = "[";
    for (int i = 0; i < (int)v.size(); i++) { if (i) s += ", "; s += "\"" + v[i] + "\""; }
    return s + "]";
}

static void check(const char* name, std::vector<std::string> got,
                  std::vector<std::string> expected, const char* input) {
    std::sort(got.begin(), got.end());
    std::sort(expected.begin(), expected.end());
    std::string gs = fmtPaths(got), es = fmtPaths(expected);
    nlc_case(name, got == expected, input, es.c_str(), gs.c_str(), "");
}

int main() {
    nlc_begin();

    // Build a sample tree:
    // root/
    //   main.cpp  (size=1200)
    //   readme.md (size=500)
    //   lib/
    //     util.h    (size=300)
    //     parser.cpp (size=800)
    //   tests/
    //     test_main.cpp (size=950)
    //     data.json     (size=2000)
    //     deep/
    //       inner.cpp (size=100)

    Dir deep{"deep", {{"inner","cpp",100,0}}, {}};
    Dir tests{"tests", {{"test_main","cpp",950,0},{"data","json",2000,0}}, {deep}};
    Dir lib{"lib", {{"util","h",300,0},{"parser","cpp",800,0}}, {}};
    Dir root{"root", {{"main","cpp",1200,0},{"readme","md",500,0}}, {lib, tests}};

    // Test 1: empty directory
    {
        Dir empty{"empty", {}, {}};
        check("empty directory", findFiles(empty, {}), {}, "empty dir, no filter");
    }

    // Test 2: filter by extension cpp
    check("filter by extension cpp",
          findFiles(root, {"cpp",0,INT_MAX,""}),
          {"root/main.cpp","root/lib/parser.cpp","root/tests/test_main.cpp","root/tests/deep/inner.cpp"},
          "ext=cpp");

    // Test 3: filter by size range [400, 1000]
    // main.cpp=1200 (excluded), readme.md=500, parser.cpp=800, test_main.cpp=950 all match
    check("filter by size [400,1000]",
          findFiles(root, {"",400,1000,""}),
          {"root/readme.md","root/lib/parser.cpp","root/tests/test_main.cpp"},
          "size 400-1000");

    // Test 4: filter by name substring "main"
    check("filter by name substring 'main'",
          findFiles(root, {"",0,INT_MAX,"main"}),
          {"root/main.cpp","root/tests/test_main.cpp"},
          "name_contains=main");

    // Test 5: combined filters ext=cpp, size<=900
    // test_main.cpp=950 is excluded (too large); parser=800, inner=100 match
    check("combined ext=cpp and size<=900",
          findFiles(root, {"cpp",0,900,""}),
          {"root/lib/parser.cpp","root/tests/deep/inner.cpp"},
          "ext=cpp, max_size=900");

    // Test 6: nested deep directory
    check("deep nested directory",
          findFiles(root, {"",0,INT_MAX,"inner"}),
          {"root/tests/deep/inner.cpp"},
          "name_contains=inner (deep)");

    // Test 7: no matches
    check("no matches",
          findFiles(root, {"py",0,INT_MAX,""}),
          {},
          "ext=py, no py files");

    nlc_end();
    return 0;
}
