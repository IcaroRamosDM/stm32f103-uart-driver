#include "app/app_uart_demo.h"

#include "drivers/uart/uart.h"

#define APP_UART_DEMO_MESSAGE       "UART driver OK\r\n"

void app_uart_demo_tx_message(void) {

    uart_write_string(APP_UART_DEMO_MESSAGE);
}

void app_uart_demo_blocking_echo(void) {

    const uint8_t received_byte = uart_read_byte();

    uart_write_byte(received_byte);
}
