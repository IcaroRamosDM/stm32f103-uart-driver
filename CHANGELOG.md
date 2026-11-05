# Changelog

## Unreleased

- Use lock-free atomic indices with acquire/release ordering for the single-producer/single-consumer receive queue.
- Route blocking and nonblocking receive through the ISR queue. Blocking receive requires RX interrupts enabled.
- Preserve queued bytes on overflow, handle null output arguments and test 200000 concurrent transfers plus mocked USART ownership.
- Keep build products under build/ and document the 127-byte usable receive capacity.
