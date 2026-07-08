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

static void chk(const char* name, int got, int expected, const char* input) {
    nlc_case(name, got==expected, input,
             std::to_string(expected).c_str(), std::to_string(got).c_str(), "");
}

int main() {
    nlc_begin();

    chk("both empty", editDistance("",""), 0, "\"\",\"\"");
    chk("first empty", editDistance("","abc"), 3, "\"\",\"abc\"");
    chk("second empty", editDistance("abc",""), 3, "\"abc\",\"\"");
    chk("identical strings", editDistance("abc","abc"), 0, "\"abc\",\"abc\"");
    chk("one insertion a→ab", editDistance("a","ab"), 1, "\"a\",\"ab\"");
    chk("one substitution a→b", editDistance("a","b"), 1, "\"a\",\"b\"");
    chk("classic horse→ros", editDistance("horse","ros"), 3, "\"horse\",\"ros\"");
    chk("intention→execution", editDistance("intention","execution"), 5, "\"intention\",\"execution\"");

    nlc_end();
    return 0;
}
