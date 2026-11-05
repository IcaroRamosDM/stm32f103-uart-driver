#pragma once

#include <stdbool.h>
#include <stdint.h>

#define USART_SR_RXNE (1U << 5U)

bool usart_get_flag(uint32_t port, uint32_t flag);
