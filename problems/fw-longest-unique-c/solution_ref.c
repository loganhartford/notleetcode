// Time: O(n) — sliding window with last-seen index array
#include <stddef.h>
#include <string.h>

size_t longest_unique_substr_ascii(const char *s) {
    int last[256];
    memset(last, 0, sizeof last);
    size_t lo = 0, best = 0;
    for (size_t hi = 0; s[hi]; hi++) {
        unsigned char c = (unsigned char)s[hi];
        if (last[c] > 0 && (size_t)(last[c] - 1) >= lo) {
            lo = (size_t)last[c];
        }
        last[c] = (int)(hi + 1);
        size_t len = hi - lo + 1;
        if (len > best) best = len;
    }
    return best;
}
