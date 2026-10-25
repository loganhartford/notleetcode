# Find a Bit Pattern in a Byte Stream

A receiver hands you a buffer of bytes in network byte order (big-endian, most significant bit first). Find the first occurrence of the 32-bit pattern `0xFE6B2840` in the bit stream. The pattern does not have to be byte-aligned: it can start at any bit.

```c
int findPattern(const uint32_t numBytes, const uint8_t data[]);
```

Bits are numbered from the start of the stream: bit 0 is the most significant bit of `data[0]`, bit 7 is the least significant bit of `data[0]`, bit 8 is the most significant bit of `data[1]`, and so on.

Return:
- the bit offset where the first match starts,
- `-1` if the pattern does not occur,
- `-2` if `data` is `NULL` or `numBytes < 4`.

### Example 1
```
Input:  numBytes = 4, data = {0xFE, 0x6B, 0x28, 0x40}
Output: 0
```

### Example 2
```
Input:  numBytes = 6, data = {0x00, 0x00, 0xFE, 0x6B, 0x28, 0x40}
Output: 16
```

### Example 3
```
Input:  numBytes = 6, data = {0x00, 0x01, 0xFC, 0xD6, 0x50, 0x80}
Output: 15
Explanation:
  00000000 0000000[1 11111100 11010110 01010000 1000000]0
  The bracketed 32 bits are 0xFE6B2840, starting at bit 15.
```

### Example 4
```
Input:  numBytes = 4, data = {0xFE, 0x6B, 0x28, 0x41}
Output: -1
```

### Example 5
```
Input:  numBytes = 3, data = {0xFE, 0x6B, 0x28}
Output: -2
```

### Constraints
- `0 <= numBytes <= 2^20`
- `data` may be `NULL`.
- Never read beyond `data[numBytes - 1]`.

**Optimal complexity:** O(n) time, O(1) extra space, where n = `numBytes`.

*Source: reported in an Amazon embedded software interview, Redmond ([Glassdoor](https://www.glassdoor.co.nz/Interview/Amazon-Interview-E6036-RVW102328159.htm), Oct 2025); a similar question appeared in an [Amazon online assessment](https://leetcode.com/discuss/interview-question/1672489/amazon-online-assessment-seattle).*
