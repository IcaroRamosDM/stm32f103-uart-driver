#include "drivers/uart/uart.h"

#include "bsp/usart.h"

void uart_init(void) {

    usart_setup();
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
