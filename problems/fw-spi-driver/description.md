# Fake SPI Register Driver

Implement a minimal SPI peripheral driver that operates against a memory-mapped register struct. This simulates the pattern used in real MCU HAL drivers: poll status bits, write data, poll again, read result.

The register definitions, `spi_regs_t`, and `spi_status_t` are provided.

```c
void         spi_init(spi_regs_t *regs, uint32_t ctrl_flags);
spi_status_t spi_transfer_byte(spi_regs_t *regs, uint8_t tx,
                                uint8_t *rx, uint32_t timeout_iters);
```

**`spi_init`:** set `regs->CTRL = ctrl_flags | SPI_CTRL_ENABLE`.

**`spi_transfer_byte`** implements this sequence:
1. Poll `regs->STATUS & SPI_STATUS_TX_RDY` up to `timeout_iters` times; if never set, return `SPI_TIMEOUT`
2. Write `regs->TXD = tx`
3. Poll `regs->STATUS & SPI_STATUS_RX_RDY` up to `timeout_iters` times; if never set, return `SPI_TIMEOUT`
4. If `regs->STATUS & SPI_STATUS_ERR`, return `SPI_ERROR`
5. Read `*rx = (uint8_t)regs->RXD`, return `SPI_OK`

## Examples

**Example 1** — successful transfer
```
spi_init(&regs, 0);
regs.STATUS = SPI_STATUS_TX_RDY | SPI_STATUS_RX_RDY;
regs.RXD    = 0xAB;
spi_transfer_byte(&regs, 0x55, &rx, 100) → SPI_OK, rx=0xAB
```

**Example 2** — timeout
```
regs.STATUS = 0;  // TX_RDY never set
spi_transfer_byte(&regs, 0x55, &rx, 10) → SPI_TIMEOUT
```

**Example 3** — error flag
```
regs.STATUS = SPI_STATUS_TX_RDY | SPI_STATUS_RX_RDY | SPI_STATUS_ERR;
spi_transfer_byte(&regs, 0x55, &rx, 100) → SPI_ERROR
```

## Constraints

- Poll loops decrement `timeout_iters` on each iteration; return `SPI_TIMEOUT` when counter reaches 0
- Check `SPI_STATUS_ERR` only after `RX_RDY` is observed
- `spi_init` must set the `SPI_CTRL_ENABLE` bit regardless of `ctrl_flags`

**Optimal complexity:** O(timeout_iters) worst case
