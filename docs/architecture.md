# Architecture

src/bsp configures the clock, pins and USART1 peripheral. The UART driver exposes
blocking transmit and both receive APIs. uart_buffer.c owns circular-buffer
operations; app_uart_demo.c interprets commands and controls the LED.

The receive ISR is the only producer. The main loop is the only consumer.
Do not add another producer or consumer without revising the synchronization
contract in interrupt_model.md. The buffer is initialized before enabling RX interrupts.
