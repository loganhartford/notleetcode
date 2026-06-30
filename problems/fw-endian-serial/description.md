# Endian-Safe Message Serialization

Implement encode and decode functions for a fixed-format sensor message. The wire format is always little-endian regardless of host byte order, so you must serialize field by field using explicit shifts — do not use `memcpy` on multi-byte fields or rely on struct layout.

The `sensor_msg_t` struct and `SENSOR_MSG_WIRE_LEN` constant are provided.

Wire layout (9 bytes total, all little-endian):

| Offset | Size | Field               |
|--------|------|---------------------|
| 0      | 4    | `device_id`         |
| 4      | 2    | `temperature_q8_8`  |
| 6      | 2    | `millivolts`        |
| 8      | 1    | `flags`             |

```c
size_t sensor_msg_encode(uint8_t *out, size_t out_len, const sensor_msg_t *msg);
bool   sensor_msg_decode(sensor_msg_t *msg, const uint8_t *in, size_t in_len);
```

- **encode** returns `SENSOR_MSG_WIRE_LEN` (9) on success, `0` if `out_len < 9`
- **decode** returns `false` if `in_len < 9`

## Examples

**Example 1** — encode
```
msg = { device_id=0x01020304, temperature_q8_8=0x0140, millivolts=0x0BB8, flags=0x05 }
encoded bytes: 04 03 02 01  40 01  B8 0B  05
```

**Example 2** — round-trip
```
sensor_msg_encode(buf, 9, &original);
sensor_msg_decode(&decoded, buf, 9);
// decoded == original
```

**Example 3** — error cases
```
sensor_msg_encode(buf, 8, &msg) → 0        (buffer too small)
sensor_msg_decode(&msg, buf, 8) → false    (input too short)
```

## Constraints

- Serialize with explicit byte extractions: `out[0] = val & 0xFF; out[1] = (val >> 8) & 0xFF; ...`
- `temperature_q8_8` is `int16_t` — cast carefully when encoding
- `out_len < SENSOR_MSG_WIRE_LEN` → return 0; `in_len < SENSOR_MSG_WIRE_LEN` → return false

**Optimal complexity:** O(1) — fixed-size message
