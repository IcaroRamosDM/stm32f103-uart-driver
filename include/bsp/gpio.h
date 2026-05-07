#pragma once

#include <stdbool.h>

void gpio_setup(void);
void gpio_led_set(bool enabled);
void gpio_led_toggle(void);
