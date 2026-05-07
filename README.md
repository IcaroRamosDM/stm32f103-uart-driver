# STM32F103 UART Driver

Bare-metal UART driver implementation for STM32F103C8T6 (Blue Pill) using libopencm3.

This project demonstrates a complete UART architecture built from scratch, including:

- USART bring-up
- Blocking TX/RX
- Non-blocking RX using interrupt-driven ring buffer
- UART command parser
- LED control through serial terminal
- Custom startup file and vector table
- Professional firmware layering (BSP + Driver + App)

This repository is intended for:

- Embedded systems portfolio
- Firmware engineering study
- Bare-metal ARM Cortex-M learning
- UART architecture reference for future projects

---

## Features

### Core firmware

- STM32F103C8T6 bare-metal firmware
- libopencm3 framework
- Custom linker script
- Custom startup file
- Manual vector table configuration
- SysTick timing base
- GPIO abstraction layer
- Onboard LED control (PC13)

### UART architecture

- USART1 configuration (PA9 TX / PA10 RX)
- 115200 baudrate
- 8N1 communication
- Blocking UART transmit
- Blocking UART receive
- Ring buffer implementation
- RX interrupt handling (USART1 ISR)
- Non-blocking UART API

### Interactive demo

Serial terminal command parser:

- `help`
- `status`
- `led on`
- `led off`
- `blink`

---

## Hardware

### Target board

- STM32F103C8T6 (Blue Pill)

### UART interface

- USB-to-TTL serial adapter

Examples:

- FTDI
- CH340
- CP2102

---

## Wiring

### USART1

| STM32 | Function | USB-TTL |
|---|---|---|
| PA9 | TX | RX |
| PA10 | RX | TX |
| GND | GND | GND |

Important:

Do **not** connect VCC (red wire) from USB-TTL adapter.

The board should be powered separately using:

- ST-Link
- USB
- External regulated supply

This avoids voltage mismatch risks.

More details:

See:

```text
docs/wiring.md
```

---

## Build

### Requirements

- arm-none-eabi-gcc
- libopencm3
- st-flash
- picocom

### Build firmware

```bash
make clean
make
```

### Flash firmware

```bash
make flash
```

### Erase flash

```bash
make erase
```

### Reflash

```bash
make reflash
```

---

## Serial Terminal

### Install picocom

```bash
sudo apt update
sudo apt install picocom
```

### Linux serial permission

```bash
sudo usermod -aG dialout $USER
```

Then logout/login.

### Open serial terminal

```bash
picocom -b 115200 /dev/ttyUSB0
```

### Exit picocom

```text
CTRL + A
CTRL + X
```

---

## Example

After reset:

```text
UART command parser ready
>
```

Commands:

```text
> help
> status
> led on
> led off
> blink
```

---

## Project Structure

```text
include/
├── app/
├── bsp/
└── drivers/

src/
├── app/
├── bsp/
└── drivers/

startup/
linker/
docs/
```

---

## Documentation

Detailed technical explanations:

- `docs/wiring.md`
- `docs/architecture.md`
- `docs/uart_flow.md`
- `docs/interrupt_model.md`
- `docs/command_parser.md`

These documents explain:

- blocking vs non-blocking UART
- ring buffer design
- interrupt architecture
- NVIC / IRQ / ISR
- vector table design
- command parser logic
- firmware architecture decisions

---

## License

MIT License

---

## Author

**Icaro Ramos Rodrigues dos Santos**

Embedded Systems | Firmware | C | STM32 | ARM Cortex-M | Linux | Verilog | UVM | Design Verification<br><br>
GitHub: https://github.com/IcaroRamosDM<br>
LinkedIn: https://www.linkedin.com/in/icaro-ramos-r/<br>
icaroelt@gmail.com<br>
