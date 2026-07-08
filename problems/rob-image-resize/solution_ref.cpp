#include <vector>
#include <cstdint>
#include <algorithm>

std::vector<std::vector<uint8_t>> resizeNearest(
    const std::vector<std::vector<uint8_t>>& img,
    int out_h, int out_w) {
    int in_h = img.size(), in_w = img[0].size();
    std::vector<std::vector<uint8_t>> out(out_h, std::vector<uint8_t>(out_w));
    for (int r = 0; r < out_h; r++) {
        for (int c = 0; c < out_w; c++) {
            int sr = (int)((r + 0.5) * in_h / out_h);
            int sc = (int)((c + 0.5) * in_w / out_w);
            sr = std::max(0, std::min(sr, in_h - 1));
            sc = std::max(0, std::min(sc, in_w - 1));
            out[r][c] = img[sr][sc];
        }
    }
    return out;
}
