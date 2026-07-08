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

    chk("single number", calculate("3"), 3, "\"3\"");
    chk("addition", calculate("1 + 2"), 3, "\"1 + 2\"");
    chk("subtraction", calculate("10 - 3"), 7, "\"10 - 3\"");
    chk("multiplication", calculate("2 * 3"), 6, "\"2 * 3\"");
    chk("division truncate", calculate("10 / 3"), 3, "\"10 / 3\"");
    chk("mixed precedence", calculate("2 + 3 * 4"), 14, "\"2 + 3 * 4\"");
    chk("leading/trailing spaces", calculate("  3 + 5  "), 8, "\"  3 + 5  \"");
    chk("multi-digit numbers", calculate("100 + 200"), 300, "\"100 + 200\"");
    chk("negative intermediate", calculate("3 - 4 * 2"), -5, "\"3 - 4 * 2\"");

    nlc_end();
    return 0;
}
