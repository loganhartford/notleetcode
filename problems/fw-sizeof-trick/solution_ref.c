#include <stddef.h>
#include <stdint.h>

#define MY_SIZEOF_TYPE(T) \
    ((size_t)((char *)((T *)0 + 1) - (char *)((T *)0)))
