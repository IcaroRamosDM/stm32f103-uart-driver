# UART data flow

Receive: PA10 → USART1 RXNE → usart1_isr → circular buffer → main-loop consumer.
The nonblocking read returns false when no byte is available and preserves
the caller's byte. A full buffer preserves queued data and drops the newest byte.

The blocking read waits for a byte in the same queue. RX interrupts must remain
enabled while it waits. Only the ISR reads the hardware receive register, so the
blocking call cannot steal bytes from the interrupt path.

Transmit: application → uart_write_byte/string → blocking USART transmit.
Nonblocking receive does not make transmit or application commands asynchronous.
A slow consumer can fill the queue even while interrupts continue receiving.

The demonstration uses 115200 baud, 8N1. Pin and framing constants live in
include/bsp/usart_config.h. The peripheral is configured through libopencm3.
