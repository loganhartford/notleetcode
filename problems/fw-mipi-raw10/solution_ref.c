#include <stdint.h>

void mipiRaw10ToPlain16(const uint8_t *raw10,
                        uint16_t      *plain16,
                        int            pixelCount)
{
    int groups = pixelCount / 4;
    for (int g = 0; g < groups; g++) {
        const uint8_t *b = raw10 + g * 5;
        uint8_t lsbs = b[4];
        plain16[g*4 + 0] = ((uint16_t)b[0] << 2) | ((lsbs >> 0) & 0x3u);
        plain16[g*4 + 1] = ((uint16_t)b[1] << 2) | ((lsbs >> 2) & 0x3u);
        plain16[g*4 + 2] = ((uint16_t)b[2] << 2) | ((lsbs >> 4) & 0x3u);
        plain16[g*4 + 3] = ((uint16_t)b[3] << 2) | ((lsbs >> 6) & 0x3u);
    }
}
