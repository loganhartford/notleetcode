# Serial Packet Framing Parser

Implement a byte-at-a-time **framing parser** for a simple serial protocol. The
parser is a state machine fed one byte at a time (as bytes arrive from a UART) and
emits a payload whenever a complete, valid frame has been received.

### Frame format
```
[ START=0xAA ] [ LEN (1 byte) ] [ PAYLOAD (LEN bytes) ] [ CHECKSUM (1 byte) ]
```
The checksum is the **XOR of all payload bytes** (XOR of nothing = `0x00` for an
empty payload).

### API
```c
#define PKT_START 0xAA

typedef struct {
    int     state;
    uint8_t len;
    uint8_t idx;
    uint8_t buf[256];
    uint8_t cksum;
} parser_t;

void parser_init(parser_t* p);
int  parser_feed(parser_t* p, uint8_t byte, uint8_t* out, int* out_len);
```

`parser_feed` returns `1` when the byte just completed a frame **with a matching
checksum** — in that case copy the payload into `out` and set `*out_len`. On a
checksum mismatch, drop the frame (reset to waiting for START) and return `0`.
Return `0` for every byte that doesn't complete a valid frame. Bytes received
while waiting for START that aren't `0xAA` are ignored.

### Example
```
stream: AA 03 10 20 30 00   ->  delivers payload [10 20 30]   (0x10^0x20^0x30 = 0x00)
        FF                  ->  ignored (waiting for START)
        AA 01 55 55         ->  delivers payload [55]
        AA 02 01 02 FF      ->  bad checksum (should be 03), dropped
```

### Constraints
- `LEN` is `0..255`.
- `[author]`-defined firmware practice.
