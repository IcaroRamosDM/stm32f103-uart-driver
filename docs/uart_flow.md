# UART Data Flow

## Overview

This project implements UART communication in three progressive stages:

1. Blocking TX/RX
2. Ring buffer introduction
3. Interrupt-driven non-blocking RX

The objective is not only to make UART functional, but to demonstrate the architectural evolution from simple validation code to production-grade firmware design.

Each stage exists intentionally and serves a different engineering purpose.

---

## Stage 1 — Blocking UART

### TX

Initial transmission uses:

```c
usart_send_blocking(...)
```

Example:

```c
uart_write_byte('A');
```

Behavior:

```text
CPU sends byte
CPU waits until transmission finishes
CPU continues execution
```

This is a synchronous operation.

### RX

Initial reception uses:

```c
usart_recv_blocking(...)
```

Example:

```c
uint8_t byte = uart_read_byte();
```

Behavior:

```text
CPU waits
until a byte arrives
then execution continues
```

### Why Start With Blocking

Blocking UART is the correct first step for peripheral bring-up because it isolates hardware validation.

It answers:

- Is baudrate correct?
- Is TX wiring correct?
- Is RX wiring correct?
- Is USART configured correctly?

Before interrupts, buffers, and parsers, hardware must be proven functional.

This reduces debugging complexity significantly.

### Limitation

Blocking UART does not scale.

While waiting:

- CPU cannot execute other tasks
- timing-sensitive operations may be delayed
- firmware responsiveness degrades

Blocking RX is especially problematic for real systems.

---

## Stage 2 — Ring Buffer

### Motivation

To remove permanent blocking from reception, incoming data must be stored independently from application timing.

This is solved using a circular buffer (ring buffer).

### Structure

```c
typedef struct
{
    uint8_t data[UART_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
} uart_buffer_t;
```

### Concept

```text
Incoming byte
↓
stored in buffer
↓
application reads later
```

Two indexes are used:

- `head` → next write position
- `tail` → next read position

### Why Circular

When the end of the buffer is reached:

```text
index returns to zero
```

instead of stopping.

This avoids memory movement and keeps operations O(1).

### Advantages

- predictable behavior
- constant-time push/pop
- no dynamic allocation
- ideal for ISR usage

This is the standard approach for UART RX in embedded systems.

---

## Stage 3 — Interrupt-Driven RX

### Motivation

The correct place to capture incoming UART bytes is not inside `main()`.

It is inside the USART interrupt.

### Flow

```text
PC sends byte
↓
USART receives byte
↓
RXNE flag is set
↓
IRQ generated
↓
NVIC accepts interrupt
↓
Vector table calls usart1_isr()
↓
ISR reads byte
↓
ISR pushes byte into ring buffer
↓
main() processes later
```

This separates:

```text
Reception
from
Processing
```

which is the correct architecture.

### ISR

```c
void usart1_isr(void)
{
    if (usart_get_flag(BSP_USART_PORT, USART_SR_RXNE))
    {
        uart_rx_interrupt_handler();
    }
}
```

The ISR must be minimal.

Its job is:

- read byte
- store byte
- exit

It should not:

- parse commands
- print strings
- execute business logic

Heavy ISR code is poor firmware design.

### Why This Is Better

Now:

```text
main() never waits for UART
```

The CPU remains available for:

- sensors
- timers
- control loops
- protocols
- additional tasks

This is production-grade behavior.

---

## Blocking vs Non-Blocking

### Blocking

```text
CPU waits for UART
```

Best for:

- bring-up
- peripheral validation
- very simple tools

### Non-Blocking

```text
UART works in background
CPU continues executing
```

Best for:

- real products
- scalable firmware
- multi-task systems

### Engineering Rule

Correct sequence:

```text
First:
make blocking version work

Then:
migrate to interrupt-driven design
```

Never start with interrupts before proving the peripheral.

That creates unnecessary debugging complexity.

---

## Why Both Implementations Exist

This project intentionally keeps both:

- blocking UART
- interrupt-driven UART

Reason:

Blocking implementation is valuable for learning and debugging.

Interrupt-driven implementation is required for professional firmware architecture.

Both are useful.

Removing the blocking version would reduce the educational value of the project.

---

## Final Result

The final UART driver provides:

```c
uart_init();

uart_write_byte(...);
uart_write_string(...);

uart_available();
uart_read_byte_non_blocking(...);
```

with RX handled by:

```text
USART1 interrupt + ring buffer
```

This architecture is suitable for:

- CLI systems
- bootloaders
- protocol handlers
- sensor gateways
- embedded command interfaces

and serves as a strong reusable baseline for future STM32 projects.
