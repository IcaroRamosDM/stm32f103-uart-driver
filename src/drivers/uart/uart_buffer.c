#include "drivers/uart/uart_buffer.h"

void uart_buffer_init(uart_buffer_t *buffer) {

    buffer->head = 0U;
    buffer->tail = 0U;
}

bool uart_buffer_is_empty(const uart_buffer_t *buffer) {

    return (buffer->head == buffer->tail);
}

bool uart_buffer_is_full(const uart_buffer_t *buffer) {

    const uint16_t next_head =
        (buffer->head + 1U) % UART_BUFFER_SIZE;

    return (next_head == buffer->tail);
}

bool uart_buffer_push(
        uart_buffer_t *buffer,
        uint8_t byte
) {
    if (uart_buffer_is_full(buffer)) {
     
        return false;
    }

    buffer->data[buffer->head] = byte;
    buffer->head =
        (buffer->head + 1U) % UART_BUFFER_SIZE;

    return true;
}

bool uart_buffer_pop(
        uart_buffer_t *buffer,
        uint8_t *byte
) {
    if (uart_buffer_is_empty(buffer)) {

        return false;
    }

    *byte = buffer->data[buffer->tail];
    buffer->tail =
        (buffer->tail + 1U) % UART_BUFFER_SIZE;

    return true;
}
