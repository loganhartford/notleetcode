#include <vector>
#include <string>
#include <algorithm>

std::vector<std::vector<std::string>> suggestedProducts(
    std::vector<std::string> products, std::string searchWord) {
    std::sort(products.begin(), products.end());
    std::vector<std::vector<std::string>> result;
    std::string prefix;
    for (char ch : searchWord) {
        prefix += ch;
        std::vector<std::string> matches;
        auto it = std::lower_bound(products.begin(), products.end(), prefix);
        for (int i = 0; i < 3 && it != products.end(); i++, ++it) {
            if (it->substr(0, prefix.size()) == prefix) {
                matches.push_back(*it);
            } else {
                break;
            }
        }
        result.push_back(matches);
    }
    return result;
}
