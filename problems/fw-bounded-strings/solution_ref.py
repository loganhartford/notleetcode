def c_strlcpy(dst: bytearray, src: bytes, dstsize: int) -> int:
    n = 0
    while src[n] != 0:
        if dstsize > 0 and n < dstsize - 1:
            dst[n] = src[n]
        n += 1
    if dstsize > 0:
        dst[n if n < dstsize else dstsize - 1] = 0
    return n
