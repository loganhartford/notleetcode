#include <stdint.h>

void mipiRaw10ToPlain16(const uint8_t *raw10,
                        uint16_t      *plain16,
                        int            pixelCount)
{
    for (int i = 0; i < (pixelCount / 4); i++) {
        for (int j = 0; j < 4; j++) {
            plain16[(i * 4) + j] = raw10[(i * 5) + j] << 2 | (raw10[(i * 5) + 4] >> (j * 2)) & 3U;
        }
    }
}
