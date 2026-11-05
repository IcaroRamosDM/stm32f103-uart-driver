# STM32F103 UART Driver

[![Checks](https://github.com/IcaroRamosDM/stm32f103-uart-driver/actions/workflows/checks.yml/badge.svg)](https://github.com/IcaroRamosDM/stm32f103-uart-driver/actions/workflows/checks.yml)

Bare-metal USART1 firmware using libopencm3, an interrupt-driven receive buffer
and a serial command demo. Board setup, UART access and application commands
are separate modules.

## Behavior

- USART1 on PA9/PA10, 115200 baud, 8 data bits, no parity and one stop bit.
- Blocking transmit, with blocking and nonblocking receive APIs consuming the same ISR queue.
- A 128-slot circular buffer with 127 usable bytes.
- Lock-free atomic indices between one interrupt producer and one main-loop consumer.
- A full receive buffer discards the newly arriving byte and preserves queued data.

## Build and use

[Build instructions and host verification](docs/build.md)

~~~sh
make all
make test
make sanitize
~~~

The host tests check capacity, overflow, wrapping, invalid arguments and concurrent
producer/consumer ordering. They do not emulate interrupt timing or electrical UART.

After programming the board, open a 3.3 V serial interface:

~~~sh
picocom -b 115200 /dev/ttyUSB0
~~~

Available commands: help, status, led on, led off and blink.
The blocking receive call requires RX interrupts to remain enabled. It waits for
a queued byte and must not run inside the receive ISR.

The blink command and transmit path can block the application. Receive interrupts
continue, but an application that stops draining the queue can lose new bytes.

## Documentation

- [Architecture](docs/architecture.md)
- [Interrupt and buffer contract](docs/interrupt_model.md)
- [Data flow](docs/uart_flow.md)
- [Command parser](docs/command_parser.md)
- [Wiring](docs/wiring.md)

## License

[MIT](LICENSE).

## Author

**Icaro Ramos Rodrigues dos Santos**<br>
Electronic Engineer

Embedded Systems | Firmware | C | C++ | Python | Dart | Assembly | Hardware Design | PCB Design | STM32 | ARM Cortex-M | Linux | Windows | RISC-V | Verilog | SystemVerilog | UVM | Design Verification | Instrumentation | Signal Processing | Qt | FLTK

GitHub: [IcaroRamosDM](https://github.com/IcaroRamosDM)<br>
LinkedIn: [icaro-ramos-r](https://www.linkedin.com/in/icaro-ramos-r/)<br>
[icaroelt@gmail.com](mailto:icaroelt@gmail.com)
