#include <stddef.h>
#include <stdbool.h>
#include <string.h>

size_t parse_tokens(char *line, char **argv, size_t max_args) {
    size_t n = strlen(line);
    bool in_token = false;
    size_t num_tokens = 0;
    for (int i = 0; i < n; i++) {
        if ((line[i] == ' ' || line[i] == '\t') && in_token) {
            line[i] = '\0';
            in_token = false;
        } else if (line[i] == ' ' || line[i] == '\t') {
            continue;
        }
        else if (!in_token) {
            argv[num_tokens++] = &line[i];
            in_token = true;
        }
    }
    return num_tokens > max_args ? max_args : num_tokens;
}
