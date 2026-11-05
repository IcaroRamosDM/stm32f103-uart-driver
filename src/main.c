#include "app/app_uart_demo.h"
#include "bsp/clock.h"
#include "bsp/gpio.h"
#include "bsp/systick.h"
#include "drivers/uart/uart.h"

int main(void) {

    clock_setup();
    systick_setup();
    gpio_setup();
    uart_init();

    uart_write_string("\r\nUART command parser ready\r\n> ");

    while (1) {

        app_uart_demo_command_parser();
    }

    return 0;
}
