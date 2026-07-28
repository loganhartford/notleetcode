#include <stdio.h>
#include <stdint.h>

#define FLIP_ENDIANNESS(x)                              \
    ( ((((uint32_t)(x)) & 0x000000FFu) << 24) |         \
      ((((uint32_t)(x)) & 0x0000FF00u) <<  8) |         \
      ((((uint32_t)(x)) & 0x00FF0000u) >>  8) |         \
      ((((uint32_t)(x)) & 0xFF000000u) >> 24) )
