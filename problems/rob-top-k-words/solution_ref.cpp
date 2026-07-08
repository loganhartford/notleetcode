#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

std::vector<std::string> topKFrequent(std::vector<std::string> words, int k) {
    std::unordered_map<std::string,int> freq;
    for (auto& w : words) freq[w]++;

    std::vector<std::string> unique;
    for (auto& [w,_] : freq) unique.push_back(w);

    std::sort(unique.begin(), unique.end(), [&](const std::string& a, const std::string& b){
        if (freq[a] != freq[b]) return freq[a] > freq[b]; // higher freq first
        return a < b; // lexicographic tiebreak
    });

    unique.resize(k);
    return unique;
}
