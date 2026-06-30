# Byte-Stream Packet Parser

Implement a byte-at-a-time state machine that extracts framed packets from a raw byte stream. Each byte comes in one at a time (as it would from a UART interrupt).

Frame format:
```
0xAA | LEN | PAYLOAD[LEN] | CHECKSUM
```
Checksum = low 8 bits of `LEN + sum(all payload bytes)`.

The `parser_t` struct and constants are provided. Implement:

```c
void            parser_init(parser_t *p);
parser_result_t parser_feed(parser_t *p, uint8_t byte,
                             uint8_t *payload_out, size_t *len_out);
```

- **`parser_feed`** returns `PARSER_NONE` until a complete packet is received
- On a valid packet: returns `PARSER_PACKET`, writes up to `PARSER_MAX_PAYLOAD` bytes into `payload_out`, and sets `*len_out`
- On a bad checksum or oversized `LEN` (> `PARSER_MAX_PAYLOAD`): returns `PARSER_ERROR` and resets to `WAIT_START`
- Any bytes before `0xAA` are ignored (noise tolerance)

## Examples

**Example 1** — valid 3-byte payload
```
Feed: AA 03 11 22 33 [cksum = 03+11+22+33 = 0x69]
Feed bytes one at a time → PARSER_PACKET, payload={0x11,0x22,0x33}, len=3
```

**Example 2** — noise before packet
```
Feed: DE AD BE EF AA 01 0x55 [cksum = 01+55 = 0x56]
First four bytes → PARSER_NONE each; packet found on AA
```

**Example 3** — bad checksum
```
Feed: AA 02 0x10 0x20 0xFF → PARSER_ERROR (expected 0x32, got 0xFF)
```

## Constraints

- States: WAIT_START=0, READ_LEN=1, READ_PAYLOAD=2, READ_CKSUM=3
- LEN > PARSER_MAX_PAYLOAD → return PARSER_ERROR immediately, reset to WAIT_START
- After error, parser resumes scanning for 0xAA

**Optimal complexity:** O(1) per byte
