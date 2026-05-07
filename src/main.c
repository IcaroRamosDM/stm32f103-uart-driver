#include "app/app_uart_demo.h"
#include "bsp/clock.h"
#include "bsp/gpio.h"
#include "bsp/systick.h"
#include "drivers/uart/uart.h"

#define MAIN_LOOP_DELAY_MS          1000U

int main(void) {
    
    clock_setup();
    systick_setup();
    gpio_setup();
    uart_init();

    while (1) {

        app_uart_demo_tx_message();
        gpio_led_toggle();
        delay_ms(MAIN_LOOP_DELAY_MS);
    }

    return 0;
}
