#include "drivers/uart/uart_buffer.h"

#include <stddef.h>

static unsigned next_index(unsigned index) {
    return (index + 1U) % UART_BUFFER_SIZE;
}

void uart_buffer_init(uart_buffer_t *buffer) {

    if (buffer == NULL) {
        return;
    }
    atomic_init(&buffer->head, 0U);
    atomic_init(&buffer->tail, 0U);
}

bool uart_buffer_is_empty(const uart_buffer_t *buffer) {

    if (buffer == NULL) {
        return true;
    }
    const unsigned tail = atomic_load_explicit(&buffer->tail, memory_order_relaxed);
    const unsigned head = atomic_load_explicit(&buffer->head, memory_order_acquire);
    return head == tail;
}

bool uart_buffer_is_full(const uart_buffer_t *buffer) {

    if (buffer == NULL) {
        return false;
    }
    const unsigned head = atomic_load_explicit(&buffer->head, memory_order_relaxed);
    const unsigned tail = atomic_load_explicit(&buffer->tail, memory_order_acquire);
    return next_index(head) == tail;
}

bool uart_buffer_push(uart_buffer_t *buffer, uint8_t byte) {
    if (buffer == NULL) {
        return false;
    }
    const unsigned head = atomic_load_explicit(&buffer->head, memory_order_relaxed);
    const unsigned tail = atomic_load_explicit(&buffer->tail, memory_order_acquire);
    const unsigned next_head = next_index(head);
    if (next_head == tail) {

        return false;
    }

    buffer->data[head] = byte;
    /* Publish the byte only after its storage is ready for the consumer. */
    atomic_store_explicit(&buffer->head, next_head, memory_order_release);

    return true;
}

bool uart_buffer_pop(uart_buffer_t *buffer, uint8_t *byte) {
    if (buffer == NULL || byte == NULL) {
        return false;
    }
    const unsigned tail = atomic_load_explicit(&buffer->tail, memory_order_relaxed);
    const unsigned head = atomic_load_explicit(&buffer->head, memory_order_acquire);
    if (head == tail) {

        return false;
    }

    *byte = buffer->data[tail];
    /* Release this slot after reading it, so the producer can reuse it. */
    atomic_store_explicit(&buffer->tail, next_index(tail), memory_order_release);

    return true;
}
