#pragma once

#include <stdint.h>

void usart_setup(void);

void bsp_usart_enable_rx_interrupt(void);

void usart_write_byte(uint8_t data);
uint8_t usart_read_byte(void);

uint8_t usart_read_received_data_register(void);
