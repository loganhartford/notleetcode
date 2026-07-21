# SENT Angle Sensor Decode

**SENT** (Single Edge Nibble Transmission) is a one-way protocol an angle sensor uses to
stream readings to an MCU over a single wire, encoding values as pulse widths (measured
in clock *ticks*).

A GPIO ISR timestamps each rising edge and stores the pulse widths in a ring buffer. A
periodic `dev_angleSensor_run1ms` task then decodes whatever whole messages have landed
in the buffer. You implement that decode step.

### Message format

One message is **4 pulses**: a sync pulse followed by three data pulses.

```
     SYNC          DATA1     DATA2         DATA3
  [56 ticks]        [15]      [22]          [17]
 ________________   _______   ___________   __________
|                |_|       |_|           |_|          |_
```

- **SYNC pulse** = exactly `56` ticks. It marks the start of a message.
- **DATA pulse** = 12–27 ticks, encoding a 4-bit nibble: `nibble = pulse_ticks - 12`.

The angle lives in **DATA2** and **DATA3**:

```
Fast-channel raw value (7 bits) = (DATA2 << 3) | (DATA3 >> 1)
Angle                           = raw * 2.8125
```

That is: the 4 bits of DATA2 form the high bits, and the top 3 bits of DATA3 (drop its
lowest bit) form the low bits.

### Worked example

```
DATA2 = 22 ticks -> 22-12 = 10 = 0b1010
DATA3 = 17 ticks -> 17-12 =  5 = 0b0101

raw   = (0b1010 << 3) | (0b0101 >> 1)
      =  0b1010000     | 0b010
      =  0b1010010     = 82
angle = 82 * 2.8125    = 230.625
```

### What you implement

```c
float dev_angleSensor_decode(const uint8_t *pulse_buffer, uint8_t length);
```

Scan the buffer for sync pulses. For **every complete message** (a sync pulse with all
three of its data pulses inside the buffer), decode the angle as above. Return the
**mean angle** across all complete messages. If there are none, return `0.0f`.

A message is complete only if its DATA3 pulse is still within `length` — a sync pulse too
close to the end of the buffer (its message not fully captured yet) is skipped.

### Example
```
buffer = { 27, 56, 22, 25, 18, 56, 23, 24, 21, 56, 22, 24, 13, 56, 22, 23 }  (length 16)

sync at index 1  -> DATA2=25->13, DATA3=18->6  -> raw=(13<<3)|(6>>1)=107 -> 300.9375
sync at index 5  -> DATA2=24->12, DATA3=21->9  -> raw=(12<<3)|(9>>1)=100 -> 281.25
sync at index 9  -> DATA2=24->12, DATA3=13->1  -> raw=(12<<3)|(1>>1)= 96 -> 270.0
sync at index 13 -> DATA3 would be index 16, past the end -> incomplete, skipped

mean = (300.9375 + 281.25 + 270.0) / 3 = 284.0625
```

### Design discussion (worth thinking about, not graded)

- **Which is more frequent — a new SENT message, or the 1 ms task?** A message is
  276–411 µs long, so several arrive per millisecond. The task therefore usually decodes
  **more than one** message per run — hence averaging.
- **Why the two-stage design (ISR stores pulse widths → task decodes)?** The ISR must be
  minimal so it never misses an edge; parsing/multiplies are deferred to the task.
- **How long should the buffer be?** Enough to hold every pulse that can arrive between
  task runs, with margin: ~1 ms / ~0.3 ms ≈ 3–4 messages × 4 pulses ≈ 16.

### Constraints
- Pulse widths are whole ticks; sync = 56, data pulses are 12–27.
- `0 <= length <= 64`.
- A data pulse never equals 56, so a value of 56 always means "sync."

**Optimal complexity:** O(n) — one scan of the buffer.
