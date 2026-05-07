#pragma once

#include <stdbool.h>
#include <stdint.h>

void uart_init(void);

void uart_write_byte(uint8_t data);
void uart_write_string(const char *text);

uint8_t uart_read_byte(void);

bool uart_available(void);
bool uart_read_byte_non_blocking(uint8_t *data);

void uart_rx_interrupt_handler(void);
