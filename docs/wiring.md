# Wiring

| STM32F103 | 3.3 V USB-to-TTL interface |
|---|---|
| PA9 / USART1 TX | RX |
| PA10 / USART1 RX | TX |
| GND | GND |

Power the board from its intended source. Do not connect the adapter's VCC lead
when the board is powered separately. Use compatible 3.3 V logic levels and a
common ground. Open the corresponding serial device at 115200 baud, 8N1.
