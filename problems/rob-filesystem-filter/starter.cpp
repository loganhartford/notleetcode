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

std::vector<std::string> findFiles(const Dir& root, const Filter& filter) {
    // TODO: DFS through the directory tree; collect paths of files matching filter
    // Path format: "root/subdir/file.ext"
    return {};
}
