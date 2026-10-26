# Detect Endianness and Implement htonl

A multi-byte integer can be stored with its least significant byte at the lowest address (little-endian) or its most significant byte at the lowest address (big-endian). Network protocols always transmit big-endian ("network byte order"), so code that talks to the network must convert from whatever order the host uses.

Implement four functions:

```c
int      is_little_endian_layout(const void *one);
int      is_little_endian(void);
uint32_t to_network_order(uint32_t x, int host_is_little);
uint32_t my_htonl(uint32_t x);
```

- `is_little_endian_layout`: `one` points at the 4 bytes some machine uses to store the `uint32_t` value `1`. Return `1` if that machine is little-endian, `0` if it is big-endian.
- `is_little_endian`: return `1` if the machine running this code is little-endian, `0` if it is big-endian, determined at runtime. Build it on `is_little_endian_layout`.
- `to_network_order`: convert `x` from host byte order to network byte order, where `host_is_little` (`1` or `0`) says which order the host uses. On a big-endian host, a value is already in network order.
- `my_htonl`: `to_network_order` for the machine running this code.

Taking the byte order as a parameter is what lets one machine test both cases. It is the same technique used to unit-test big-endian target code on a little-endian laptop.

### Example 1
```
is_little_endian_layout(bytes {01 00 00 00})  -> 1
is_little_endian_layout(bytes {00 00 00 01})  -> 0
```

### Example 2
```
to_network_order(0x12345678, 1)  -> 0x78563412
to_network_order(0x12345678, 0)  -> 0x12345678
```

### Example 3
```
On a little-endian host:
is_little_endian()     -> 1
my_htonl(0x000000FF)   -> 0xFF000000   (bytes in memory: 00 00 00 FF)
```

### Constraints
- No compile-time endianness checks: no `__BYTE_ORDER__`, `BYTE_ORDER`, `<endian.h>`, or similar macros.
- Do not call `htonl`, `ntohl`, `__builtin_bswap32`, or any other library byte-swap routine.
- The tests run on a little-endian machine, so `is_little_endian` and `my_htonl` are checked only against it. `is_little_endian_layout` and `to_network_order` are checked with both byte orders.

**Optimal complexity:** O(1) time and space.

*Source: reported in an [NVIDIA interview](https://www.glassdoor.ca/Interview/NVIDIA-Interview-E7633-RVW1331622.htm) (Glassdoor) and a Signify interview; a classic from [Programming Interviews Exposed, 3rd ed.](https://www.oreilly.com/library/view/programming-interviews-exposed/9781118283400/h1-261361c13-0004.html) (Wiley).*
