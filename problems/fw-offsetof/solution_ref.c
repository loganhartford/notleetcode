#include <stddef.h>

#define MY_OFFSETOF(type, member) ((size_t)&(((type *)0)->member))

#define MY_CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr) - MY_OFFSETOF(type, member)))
