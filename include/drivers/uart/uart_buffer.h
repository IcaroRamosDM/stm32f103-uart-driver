#pragma once

#include <stdbool.h>
#include <stdint.h>

#define UART_BUFFER_SIZE        128U

typedef struct {
    uint8_t data[UART_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
} uart_buffer_t;

void uart_buffer_init(uart_buffer_t *buffer);

bool uart_buffer_is_empty(const uart_buffer_t *buffer);
bool uart_buffer_is_full(const uart_buffer_t *buffer);

bool uart_buffer_push(
        uart_buffer_t *buffer,
        uint8_t byte
);

bool uart_buffer_pop(
        uart_buffer_t *buffer,
        uint8_t *byte
);
