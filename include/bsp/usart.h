#pragma once

#include <stdint.h>

void usart_setup(void);
void usart_write_byte(uint8_t data);
uint8_t usart_read_byte(void);
