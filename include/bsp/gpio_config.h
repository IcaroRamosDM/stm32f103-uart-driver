#pragma once

#include <stdbool.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#define GPIO_LED_RCC                RCC_GPIOC
#define GPIO_LED_PORT               GPIOC
#define GPIO_LED_PIN                GPIO13
#define GPIO_LED_ACTIVE_LOW         true
