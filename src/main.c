#include "bsp/clock.h"
#include "bsp/gpio.h"
#include "bsp/systick.h"
#include "bsp/usart.h"

#define MAIN_LOOP_DELAY_MS          1000U

int main(void) {
    
    clock_setup();
    systick_setup();
    gpio_setup();
    usart_setup();

    while (1) {

        usart_write_byte('U');
        gpio_led_toggle();
        delay_ms(MAIN_LOOP_DELAY_MS);
    }

    return 0;
}
