# ADC Code to Bipolar Voltage

A 12-bit ADC is wired to a bipolar front end. The converter hands you a raw `uint16_t` code in
the range `[0, 4095]`, and that code maps **linearly** onto the analog input range
**-5.0 V to +5.0 V**:

```
code 0     ->  -5.000000 V
code 4095  ->  +5.000000 V
```

Both endpoints are inclusive, so the span of 10 V is divided across `4095` steps, not `4096`.
One LSB is `10 / 4095 = 2.442 mV`.

Implement the conversion twice — once the obvious way, once the way you would ship it on a part
with no FPU:

```c
float   adc_to_volts(uint16_t code);   /* floating point result, in volts      */
int32_t adc_to_uv(uint16_t code);      /* integer fixed-point result, in microvolts */
```

And write your own driver so you can poke at whatever codes you care about:

```c
void demo(void);   /* called once before the tests; anything you print shows up in Results */
```

### Example

```
adc_to_volts(0)     ->  -5.000000
adc_to_volts(4095)  ->   5.000000
adc_to_volts(2047)  ->  -0.001221     /* midscale is NOT exactly zero */
adc_to_volts(2048)  ->   0.001221

adc_to_uv(0)        ->  -5000000
adc_to_uv(2048)     ->     1221
adc_to_uv(4095)     ->   5000000
```

Note the third line. With an inclusive-endpoint mapping over an even number of codes there is
no code that lands on 0 V — the two center codes straddle it by half an LSB. Getting this
wrong (dividing by `4096`, or by `4095` but subtracting the wrong offset) is the usual way this
problem is failed.

### Constraints

- `code` is a `uint16_t`, so it can carry values above 4095. Clamp anything `> 4095` to 4095.
- `adc_to_volts` must be within `1e-5` V of the exact value.
- `adc_to_uv` must be within `2` µV of the exact value — rounding mode is yours to pick, but
  truncation error must not compound.
- `adc_to_uv` must not use `float` or `double` anywhere, including in intermediates.

### Things to think about

**Sizing the fixed-point intermediate.** The scale factor `10 / 4095` is not representable
exactly in binary, so you have to multiply first and divide second. That product is the whole
problem: `4095 * 10000000` is roughly `4.1e10`, which overflows a 32-bit register. Work out
what width you actually need before you write the expression, and be explicit about it — a bare
`code * 10000000` promotes to `int`, silently wraps, and the failure only shows up at the top of
the range where a technician is least likely to be looking.

**Picking the output scale.** Microvolts is one choice; Q16.16 volts is another. The test is
whether the quantum of your representation is small compared to one ADC LSB (2442 µV). Integer
millivolts would be a bad choice — a 1 mV quantum is 40% of an LSB, so you would throw away
resolution the hardware paid for. Microvolts leaves ~2400 representable steps per LSB, and
±5,000,000 fits comfortably in `int32_t`.

**FPU or not.** On a Cortex-M4F the `float` version is a couple of cycles and is the right
answer. On an M0+ every `float` operation traps into a soft-float library — hundreds of cycles,
plus a few KB of flash you may not have. Worse, if this runs in an ISR on a part where the FPU
context is lazily stacked, touching a float in the handler can force an FPU state save you did
not budget for. The integer path is deterministic, cheap, and interrupt-safe.

**Strength reduction.** If the divisor is a compile-time constant, the compiler turns the
division into a multiply-and-shift for you. If you find yourself hand-rolling that
transformation, check the disassembly first — it is usually already done.

**Optimal complexity:** O(1) — one multiply, one divide, one offset.
