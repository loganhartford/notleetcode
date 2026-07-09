#include <stdint.h>
#include <stddef.h>

/*
 * Cast NULL to T*, advance by 1, then cast back to char* and read the
 * byte offset — all at compile time, no allocation.
 */
#define MY_SIZEOF(T)  ((size_t)((char *)((T *)0 + 1) - (char *)(T *)0))
