#include <stdint.h>
#include <stddef.h>

/*
 * Define MY_SIZEOF(T) so it returns the byte size of type T
 * without using the sizeof operator.
 */
#define MY_SIZEOF(T) ((size_t)((char *)((T*)0+1)-(char *)(T *)0))

#define PTR_TO_T_ZERO(T)        ((T *)0)
#define PTR_TO_T_ONE_PAST(T)    (PTR_TO_T_ZERO(T) + 1)

#define BYTE_PTR_ZERO(T)        ((char *)PTR_TO_T_ZERO(T))
#define BYTE_PTR_ONE_PAST(T)    ((char *)PTR_TO_T_ONE_PAST(T))

#define MY_SIZEOF(T)            ((size_t)(BYTE_PTR_ONE_PAST(T) - BYTE_PTR_ZERO(T)))
