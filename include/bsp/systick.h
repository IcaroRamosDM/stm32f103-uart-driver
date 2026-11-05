#pragma once

#include <stdint.h>

#include "bsp/clock_config.h"

#define SYSTICK_FREQUENCY_HZ 1000U
#define SYSTICK_RELOAD_VALUE ((CLOCK_FREQUENCY_HZ / SYSTICK_FREQUENCY_HZ) - 1U)

void systick_setup(void);
void delay_ms(uint32_t delay_ms_value);
