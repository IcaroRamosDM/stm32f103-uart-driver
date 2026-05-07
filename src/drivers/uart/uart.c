#include "drivers/uart/uart.h"

#include "bsp/usart.h"
#include "bsp/usart_config.h"
#include "drivers/uart/uart_buffer.h"

#include <libopencm3/stm32/usart.h>

#include <stdbool.h>
#include <stdint.h>

static uart_buffer_t uart_rx_buffer;

void uart_init(void) {

    uart_buffer_init(&uart_rx_buffer);

    usart_setup();
    bsp_usart_enable_rx_interrupt();
}

void uart_write_byte(uint8_t data) {

    usart_write_byte(data);
}

void uart_write_string(const char *text) {

    while (*text != '\0') {

        uart_write_byte((uint8_t)*text);
        text++;
    }
}

uint8_t uart_read_byte(void) {

    return usart_read_byte();
}

bool uart_available(void) {

    return !uart_buffer_is_empty(&uart_rx_buffer);
}

bool uart_read_byte_non_blocking(uint8_t *data) {

    return uart_buffer_pop(&uart_rx_buffer, data);
}

void uart_rx_interrupt_handler(void) {

    const uint8_t received_byte = usart_read_received_data_register();

    (void)uart_buffer_push(&uart_rx_buffer, received_byte);
}

void usart1_isr(void) {

    if (usart_get_flag(BSP_USART_PORT, USART_SR_RXNE)) {

        uart_rx_interrupt_handler();
    }
}
