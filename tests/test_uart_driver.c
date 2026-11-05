#include "drivers/uart/uart.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static bool configured;
static bool interrupt_enabled;
static uint8_t received_register;
static unsigned polling_reads;
static unsigned transmitted_count;
static uint8_t transmitted[8];

static void check(bool condition, const char *expression, int line) {
    if (!condition) {
        fprintf(stderr, "test_uart_driver.c:%d: %s\n", line, expression);
        exit(EXIT_FAILURE);
    }
}

#define CHECK(condition) check((condition), #condition, __LINE__)

void usart_setup(void) {
    configured = true;
}

void bsp_usart_enable_rx_interrupt(void) {
    CHECK(configured);
    interrupt_enabled = true;
}

void usart_write_byte(uint8_t byte) {
    CHECK(transmitted_count < sizeof transmitted);
    transmitted[transmitted_count++] = byte;
}

uint8_t usart_read_byte(void) {
    polling_reads++;
    return 0xFFU;
}

uint8_t usart_read_received_data_register(void) {
    return received_register;
}

bool usart_get_flag(uint32_t port, uint32_t flag) {
    (void)port;
    (void)flag;
    return true;
}

int main(void) {
    uart_init();
    CHECK(interrupt_enabled);
    received_register = 0x42U;
    uart_rx_interrupt_handler();
    CHECK(uart_available());
    uint8_t byte = uart_read_byte();
    CHECK(byte == 0x42U);
    CHECK(polling_reads == 0U);
    CHECK(!uart_available());

    received_register = 0x17U;
    uart_rx_interrupt_handler();
    bool result = uart_read_byte_non_blocking(&byte);
    CHECK(result && byte == 0x17U);
    result = uart_read_byte_non_blocking(&byte);
    CHECK(!result);

    uart_write_string(NULL);
    CHECK(transmitted_count == 0U);
    uart_write_string("OK");
    CHECK(transmitted_count == 2U);
    CHECK(transmitted[0] == 'O' && transmitted[1] == 'K');
    puts("PASS: initialization, queued blocking RX, nonblocking RX and transmit");
    return EXIT_SUCCESS;
}
