# Interrupt and buffer contract

USART1 RXNE triggers usart1_isr. The handler reads the received byte and attempts
to append it to the queue. A full queue rejects the new byte; it does not overwrite
unread data. The current ISR discards the rejection and provides no overflow counter.

The 128-slot ring reserves one slot to distinguish full from empty, leaving 127
usable bytes. Each index has one writer: the ISR publishes head and the main loop
publishes tail. Lock-free C17 atomic indices use release stores and acquire loads
so payload writes become visible before head advances, and a slot is reused only
after the consumer releases it. The build rejects non-lock-free atomic int support.

This contract supports one producer and one consumer. It does not provide a
multi-producer queue, DMA ownership or an RTOS synchronization abstraction.
Resetting the buffer while either participant is active is unsupported.

Host tests use a producer thread and a consumer thread, verify 200000 ordered
transfers and exercise capacity, overflow, wrapping and invalid arguments.
That test checks the shared-memory contract; it does not reproduce STM32 IRQ timing.
