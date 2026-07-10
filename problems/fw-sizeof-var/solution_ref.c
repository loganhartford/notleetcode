#include <stddef.h>
#include <stdint.h>

#define MY_SIZEOF_VAR(x) \
    ((size_t)((char *)(&(x) + 1) - (char *)&(x)))
