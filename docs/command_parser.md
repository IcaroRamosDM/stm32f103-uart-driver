# UART Command Parser

## Overview

The final application layer of this project implements a serial command parser over UART.

Instead of a simple echo test, the firmware accepts textual commands from a serial terminal and executes actions based on received input.

This transforms the project from a peripheral validation example into an interactive firmware interface.

This is significantly more valuable for:

- firmware demonstrations
- debugging
- field diagnostics
- bootloader interfaces
- embedded CLI systems

The parser operates over:

```text
USART1 + Interrupt RX + Ring Buffer
```

which means command handling is fully non-blocking.

---

## Supported Commands

Current implemented commands:

```text
help
status
led on
led off
blink
```

These commands are intentionally simple because the objective is architectural demonstration, not application complexity.

The parser structure is designed to scale for future commands.

---

## Input Flow

Execution flow:

```text
PC keyboard input
↓
USB-TTL adapter
↓
USART1 RX (PA10)
↓
USART interrupt
↓
ISR stores byte into ring buffer
↓
main() reads buffered bytes
↓
command parser builds command string
↓
command is executed
↓
response is sent back via UART
```

This architecture separates reception from interpretation.

---

## Command Buffer

The parser stores incoming bytes into:

```c
static char command_buffer[32];
```

with:

```c
static uint8_t command_index;
```

Each received character is appended until:

```text
CR (\r)
or
LF (\n)
```

is received.

At that point:

```text
the command is considered complete
```

and processing begins.

This is standard UART CLI behavior.

---

## Why Echo Characters Back

Serial terminals like `picocom` often use:

```text
local echo = off
```

which means typed characters are not shown locally.

To improve usability, the firmware echoes received characters back through UART.

This allows:

```text
user sees exactly what was typed
```

and makes command interaction significantly better.

This is especially important for demonstration and debugging.

---

## Backspace Handling

The parser explicitly handles:

```text
Backspace (0x08)
Delete (0x7F)
```

This prevents malformed commands during terminal editing.

Without this, command-line interaction becomes frustrating and error-prone.

This small detail greatly improves usability.

---

## Why Commands Are Processed in main()

The parser does not run inside the ISR.

This is intentional.

ISR responsibilities are limited to:

- read byte
- push byte to ring buffer
- exit immediately

Command parsing happens later inside:

```c
main()
```

This avoids:

- long interrupt latency
- nested interrupt problems
- unpredictable timing
- poor ISR design

This is the correct architectural decision.

---

## Command Dispatch

Commands are processed using:

```c
strcmp(...)
```

Example:

```c
if (strcmp(command, "status") == 0)
{
    uart_write_string("System OK");
}
```

This approach is intentionally simple and readable.

For larger systems, alternatives may include:

- lookup tables
- token parsers
- finite state machines
- protocol parsers

For this project, direct comparison is the correct trade-off.

It keeps the firmware clear and easy to study.

---

## LED Commands

### led on

Forces LED state to ON.

### led off

Forces LED state to OFF.

### blink

Performs visible LED blinking using:

```text
3 cycles
200 ms delay
```
---

## Why This Matters

A simple UART echo proves:

```text
the peripheral works
```

A command parser proves:

```text
the firmware architecture works
```

This is a much stronger demonstration.

It shows:

- driver quality
- interrupt correctness
- application structure
- serial interface design
- embedded system usability

---

## Final Result

The project behaves as a small embedded CLI system.

Example:

```text
UART command parser ready
> help
Commands:
help
status
led on
led off
blink

> blink
BLINK DONE
>
```

This creates a demonstration of interrupt-driven UART firmware on STM32.

It is simple, practical, and reusable for future embedded systems.
