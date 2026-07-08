# Filesystem Filter

Traverse a directory tree and return the full paths of all files matching a given filter. Multiple filter conditions are combined with **AND** — a file must satisfy all non-empty conditions.

```cpp
struct File {
    std::string name;
    std::string ext;
    int size;      // bytes
    int modified;  // unix-style integer timestamp
};

struct Dir {
    std::string name;
    std::vector<File> files;
    std::vector<Dir> subdirs;
};

struct Filter {
    std::string ext;          // match if empty or file.ext == this
    int min_size = 0;
    int max_size = INT_MAX;
    std::string name_contains; // match if empty or file.name contains this
};

// Returns full paths like "root/subdir/file.txt"
std::vector<std::string> findFiles(const Dir& root, const Filter& filter);
```

Paths are built by joining directory names with `/`. The root directory's name is included in the path.

## Example

```
Dir root { "src",
    files: [{"main", "cpp", 1200, 0}],
    subdirs: [
        Dir { "lib", files: [{"util", "h", 300, 0}], subdirs: [] }
    ]
};
Filter f { ext:"cpp" };
findFiles(root, f) → ["src/main.cpp"]
```

## Constraints

- Max depth: 20
- Max files per directory: 1000
- File path format: `dir1/dir2/filename.ext`

**Optimal complexity:** O(n) — single DFS, n = total number of files.
