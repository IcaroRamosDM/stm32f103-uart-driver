#include "bsp/systick.h"

#include <libopencm3/cm3/systick.h>

static volatile uint32_t system_millis = 0U;

void sys_tick_handler(void) {
    system_millis++;
}

void systick_setup(void) {

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(SYSTICK_RELOAD_VALUE);
    systick_interrupt_enable();
    systick_counter_enable();
}

void delay_ms(uint32_t delay_ms_value) {

    const uint32_t start_time = system_millis;

    while ((system_millis - start_time) < delay_ms_value) {
    }
}
