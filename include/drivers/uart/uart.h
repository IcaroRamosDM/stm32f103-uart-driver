#pragma once

#include <stdint.h>

void uart_init(void);
void uart_write_byte(uint8_t data);
void uart_write_string(const char *text);
