# Firmware Architecture

## Overview

This project adopts a layered firmware architecture for STM32F103 bare-metal development using libopencm3.

The objective is to separate hardware access, reusable peripheral abstractions, and application behavior into independent layers, improving maintainability, scalability, and long-term reuse.

The architecture is divided into three layers:

```text
Application Layer
Driver Layer
BSP (Board Support Package)
Hardware
```

This separation prevents tight coupling between business logic and MCU-specific implementation.

---

## BSP — Board Support Package

### Scope

The BSP layer is responsible for direct interaction with MCU peripherals and hardware configuration.

It contains:

- clock initialization
- GPIO configuration
- SysTick setup
- USART peripheral configuration
- interrupt enable/disable at peripheral level

Directory structure:

```text
include/bsp/
src/bsp/
```

Examples:

```c
clock_setup();
gpio_setup();
usart_setup();
systick_setup();
```

### Design Rule

The BSP must expose hardware operations only.

It should not contain application decisions, command parsing, protocol interpretation, or product logic.

Correct:

```c
gpio_led_set(true);
```

Incorrect:

```c
if (command == "led on")
```

The BSP answers:

```text
How does the hardware work?
```

not:

```text
What should the product do?
```

### Rationale

Keeping BSP isolated allows:

- migration to another MCU with minimal impact
- simpler peripheral debugging
- hardware validation independent of application logic

---

## Driver Layer

### Scope

The Driver layer creates reusable abstractions above BSP.

It converts peripheral-level operations into firmware modules suitable for application use.

Directory structure:

```text
include/drivers/
src/drivers/
```

Examples:

- UART driver
- ring buffer
- display drivers
- communication protocols

Instead of exposing:

```c
usart_send_blocking(...)
```

the project exposes:

```c
uart_write_string(...)
```

This removes register-level details from upper layers.

### Design Rule

Drivers should provide reusable services, not application rules.

Correct:

```c
uart_read_byte_non_blocking(...)
```

Incorrect:

```c
if (command == "status")
```

The Driver answers:

```text
How is the service provided?
```

not:

```text
What should be done with the service?
```

### Rationale

This improves:

- API clarity
- code reuse
- portability
- testability
- separation of responsibilities

---

## Application Layer

### Scope

The Application layer contains product behavior and firmware decisions.

Directory structure:

```text
include/app/
src/app/
```

Examples:

- UART command parser
- demo applications
- command execution
- feature orchestration

Example:

```c
if (strcmp(command, "led on") == 0)
{
    gpio_led_set(true);
}
```

This belongs to the application layer because it represents behavior, not hardware.

### Design Rule

The application layer decides system behavior using services provided by lower layers.

It should not configure registers directly.

Correct:

```c
gpio_led_set(true);
```

Incorrect:

```c
GPIOC_BSRR = ...
```

### Rationale

This keeps product logic independent from MCU implementation details.

---

## main.c Responsibilities

`main.c` is intentionally minimal.

Its responsibilities are:

- system initialization
- application startup
- infinite loop scheduling

Example:

```c
int main(void)
{
    clock_setup();
    systick_setup();
    gpio_setup();
    uart_init();

    while (1)
    {
        app_uart_demo_command_parser();
    }
}
```

`main.c` should orchestrate execution, not implement features.

Large business logic inside `main.c` is treated as an architectural failure.

---

## Architectural Decision: Interrupt-Driven RX

UART RX was implemented using:

- USART RX interrupt
- ring buffer
- non-blocking application access

instead of permanent blocking reads.

Reason:

Blocking RX is useful for peripheral validation but scales poorly in real firmware because CPU execution stops while waiting for serial input.

Interrupt-driven RX allows:

- continuous reception
- no lost bytes during processing
- responsive firmware behavior
- clean separation between reception and command processing

This project intentionally keeps both implementations:

- blocking RX for educational comparison
- interrupt-driven RX for production-quality architecture

---

## Architectural Decision: Custom Startup and Vector Table

The project uses a manually maintained startup file and vector table instead of generated startup code.

Reason:

This provides explicit control over:

- reset flow
- stack initialization
- interrupt vector placement
- ISR ownership

It also makes interrupt debugging significantly easier and improves understanding of Cortex-M execution flow.

This is especially important for validating:

```text
USART1 IRQ → NVIC → Vector Table → ISR → Ring Buffer
```

which is a core part of this project.

---

## Result

This architecture allows the project to function both as:

- reusable firmware reference
- portfolio-grade engineering project
- future production baseline for UART-based systems

The goal is not only to make UART work, but to structure firmware the same way professional embedded systems are built.
