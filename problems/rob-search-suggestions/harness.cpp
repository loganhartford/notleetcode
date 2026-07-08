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

static std::string fmtVV(const std::vector<std::vector<std::string>>& v) {
    std::string s = "[";
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) s += ",";
        s += "[";
        for (int j = 0; j < (int)v[i].size(); j++) {
            if (j) s += ",";
            s += "\"" + v[i][j] + "\"";
        }
        s += "]";
    }
    s += "]";
    return s;
}

static void check(const char* name,
                  std::vector<std::vector<std::string>> got,
                  std::vector<std::vector<std::string>> expected,
                  const char* input) {
    std::string gs = fmtVV(got), es = fmtVV(expected);
    nlc_case(name, got == expected, input, es.c_str(), gs.c_str(), "");
}

int main() {
    nlc_begin();

    // Test 1: basic case with 3+ matches
    check("basic case",
          suggestedProducts({"mobile","mouse","moneypot","monitor","mousepad"}, "mouse"),
          {{"mobile","moneypot","monitor"},{"mobile","moneypot","monitor"},
           {"mouse","mousepad"},{"mouse","mousepad"},{"mouse","mousepad"}},
          "products=[mobile,mouse,...] search=mouse");

    // Test 2: fewer than 3 matches for some prefix
    check("fewer than 3 matches",
          suggestedProducts({"bags","baggage","banner","box","cloths"}, "bags"),
          {{"baggage","bags","banner"},{"baggage","bags","banner"},{"baggage","bags"},{"bags"}},
          "products=[bags,...] search=bags");

    // Test 3: no matches after a prefix
    check("no matches after prefix",
          suggestedProducts({"apple","banana"}, "xyz"),
          {{},{},{}},
          "products=[apple,banana] search=xyz");

    // Test 4: prefix equal to whole word (exact match)
    check("exact match prefix",
          suggestedProducts({"apple"}, "apple"),
          {{"apple"},{"apple"},{"apple"},{"apple"},{"apple"}},
          "single product exact match");

    // Test 5: duplicate words in products
    check("duplicates handled",
          suggestedProducts({"abc","abc","abd"}, "ab"),
          {{"abc","abc","abd"},{"abc","abc","abd"}},
          "products with duplicates");

    // Test 6: single character search word
    check("single char search",
          suggestedProducts({"cat","car","card"}, "c"),
          {{"car","card","cat"}},
          "search word length 1");

    // Test 7: all products with same prefix
    check("all same prefix",
          suggestedProducts({"ab","abc","abcd","abcde","abcdef"}, "abc"),
          {{"ab","abc","abcd"},{"ab","abc","abcd"},{"abc","abcd","abcde"}},
          "all share prefix 'ab'");

    nlc_end();
    return 0;
}
