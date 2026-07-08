#include <vector>
#include <cstdint>
#include <algorithm>

std::vector<std::vector<uint8_t>> resizeNearest(
    const std::vector<std::vector<uint8_t>>& img,
    int out_h, int out_w) {
    // TODO: for each output pixel (r,c), compute:
    //   src_r = (int)((r + 0.5) * in_h / out_h)  clamped to [0, in_h-1]
    //   src_c = (int)((c + 0.5) * in_w / out_w)  clamped to [0, in_w-1]
    return {};
}
