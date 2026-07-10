#include <stdint.h>
#include <stddef.h>

/*
 * Define MY_SIZEOF_TYPE(T) so it returns the byte size of type T
 * without using the sizeof operator.
 */


/*
 * So basically we cast a pretend address 0 to "T", which is a type in this case.
 * Let's say that it's and int. then we have (int *)0, so we have an int pointer
 * at address 0.
 * 
 * Then we do the same thing and increment by 1. Pointer arithmetic moves the pointer
 * by the size of the type. so for and int, (int *)0 + 1 = 4 on a 32 bit system.
 * 
 * Then we cast to a char * because char pointers are a single byte so the arithmetic will
 * be byte based. When we subract them, we'll bet 4 - 0 = 4 which is the byte length of the int.
 */

#define MY_SIZEOF_TYPE(T) ((size_t)(((char *)(((T *)0) + 1)) - ((char *)((T *) 0))))
