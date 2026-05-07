#include "app/app_uart_demo.h"
#include "bsp/clock.h"
#include "bsp/gpio.h"
#include "drivers/uart/uart.h"

#define MAIN_LOOP_DELAY_MS          1000U

int main(void) {
    
    clock_setup();
    gpio_setup();
    uart_init();

    uart_write_string("UART interrupt echo ready\r\n");

    while (1) {

        app_uart_demo_non_blocking_echo();
    }

    return 0;
}
