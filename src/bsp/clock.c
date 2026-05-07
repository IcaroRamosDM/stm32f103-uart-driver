#include "bsp/clock.h"

#include <libopencm3/stm32/rcc.h>

void clock_setup(void) {

    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

