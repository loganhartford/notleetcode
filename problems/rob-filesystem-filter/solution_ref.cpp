#include <vector>
#include <string>
#include <climits>

struct File {
    std::string name;
    std::string ext;
    int size;
    int modified;
};

struct Dir {
    std::string name;
    std::vector<File> files;
    std::vector<Dir> subdirs;
};

struct Filter {
    std::string ext;
    int min_size = 0;
    int max_size = INT_MAX;
    std::string name_contains;
};

static void dfs(const Dir& dir, const std::string& path, const Filter& filter,
                std::vector<std::string>& result) {
    std::string cur = path.empty() ? dir.name : path + "/" + dir.name;
    for (const auto& f : dir.files) {
        if (!filter.ext.empty() && f.ext != filter.ext) continue;
        if (f.size < filter.min_size || f.size > filter.max_size) continue;
        if (!filter.name_contains.empty() && f.name.find(filter.name_contains) == std::string::npos) continue;
        std::string filepath = cur + "/" + f.name + (f.ext.empty() ? "" : "." + f.ext);
        result.push_back(filepath);
    }
    for (const auto& sub : dir.subdirs) {
        dfs(sub, cur, filter, result);
    }
}

std::vector<std::string> findFiles(const Dir& root, const Filter& filter) {
    std::vector<std::string> result;
    dfs(root, "", filter, result);
    return result;
}
