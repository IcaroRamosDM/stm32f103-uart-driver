#include "app/app_uart_demo.h"

#include "drivers/uart/uart.h"

#define APP_UART_DEMO_MESSAGE       "UART driver OK\r\n"

void app_uart_demo_tx_message(void) {

    uart_write_string(APP_UART_DEMO_MESSAGE);
}
