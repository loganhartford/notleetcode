# Detect Endianness and Implement htonl

A multi-byte integer can be stored with its least significant byte at the lowest address (little-endian) or its most significant byte at the lowest address (big-endian). Network protocols always transmit big-endian ("network byte order"), so code that talks to the network must convert from whatever order the host uses.

Implement two functions:

```c
int      is_little_endian(void);
uint32_t my_htonl(uint32_t x);
```

- `is_little_endian` returns `1` if the machine running the code is little-endian and `0` if it is big-endian. It must find out at runtime.
- `my_htonl` converts `x` from host byte order to network byte order: the returned value, as laid out in memory, must hold the bytes of `x` from most significant to least significant. It must give the correct result on both little-endian and big-endian hosts, using `is_little_endian()` to decide.

### Example 1
```
Host:   little-endian
is_little_endian()     -> 1
my_htonl(0x12345678)   -> 0x78563412   (bytes in memory: 12 34 56 78)
```

### Example 2
```
Host:   big-endian
is_little_endian()     -> 0
my_htonl(0x12345678)   -> 0x12345678   (bytes in memory: 12 34 56 78)
```

### Example 3
```
Host:   little-endian
my_htonl(0x000000FF)   -> 0xFF000000   (bytes in memory: 00 00 00 FF)
```

### Constraints
- No compile-time endianness checks: no `__BYTE_ORDER__`, `BYTE_ORDER`, `<endian.h>`, or similar macros.
- Do not call `htonl`, `ntohl`, `__builtin_bswap32`, or any other library byte-swap routine.

**Optimal complexity:** O(1) time and space.

*Source: reported in an [NVIDIA interview](https://www.glassdoor.ca/Interview/NVIDIA-Interview-E7633-RVW1331622.htm) (Glassdoor) and a Signify interview; a classic from [Programming Interviews Exposed, 3rd ed.](https://www.oreilly.com/library/view/programming-interviews-exposed/9781118283400/h1-261361c13-0004.html) (Wiley).*
