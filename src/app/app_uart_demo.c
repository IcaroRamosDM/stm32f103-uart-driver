#include "app/app_uart_demo.h"

#include "bsp/gpio.h"
#include "bsp/systick.h"
#include "drivers/uart/uart.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define APP_UART_DEMO_MESSAGE "UART driver OK\r\n"

#define APP_UART_BLINK_COUNT 3U
#define APP_UART_BLINK_DELAY_MS 200U

#define APP_UART_COMMAND_BUFFER_SIZE 32U
#define APP_UART_COMMAND_END_CR '\r'
#define APP_UART_COMMAND_END_LF '\n'
#define APP_UART_COMMAND_BACKSPACE 0x08U
#define APP_UART_COMMAND_DELETE 0x7FU
#define APP_UART_COMMAND_STRING_TERMINATOR '\0'

#define APP_UART_COMMAND_HELP "help"
#define APP_UART_COMMAND_STATUS "status"
#define APP_UART_COMMAND_LED_ON "led on"
#define APP_UART_COMMAND_LED_OFF "led off"
#define APP_UART_COMMAND_BLINK "blink"

#define APP_UART_RESPONSE_HELP "\r\nCommands:\r\nhelp\r\nstatus\r\nled on\r\nled off\r\nblink\r\n> "
#define APP_UART_RESPONSE_STATUS "\r\nSystem OK\r\n> "
#define APP_UART_RESPONSE_LED_ON "\r\nLED ON\r\n> "
#define APP_UART_RESPONSE_LED_OFF "\r\nLED OFF\r\n> "
#define APP_UART_RESPONSE_BLINK "\r\nBLINK DONE\r\n> "
#define APP_UART_RESPONSE_UNKNOWN_COMMAND "\r\nUnknown command\r\n> "
#define APP_UART_RESPONSE_BACKSPACE_SEQUENCE "\b \b"

static void app_uart_demo_process_command(const char *command);
static bool app_uart_demo_is_line_end(uint8_t byte);
static bool app_uart_demo_is_backspace(uint8_t byte);

void app_uart_demo_tx_message(void) {

    uart_write_string(APP_UART_DEMO_MESSAGE);
}

void app_uart_demo_blocking_echo(void) {

    const uint8_t received_byte = uart_read_byte();

    uart_write_byte(received_byte);
}

void app_uart_demo_non_blocking_echo(void) {

    uint8_t received_byte = 0U;

    if (uart_read_byte_non_blocking(&received_byte)) {

        uart_write_byte(received_byte);
    }
}

void app_uart_demo_command_parser(void) {

    static char command_buffer[APP_UART_COMMAND_BUFFER_SIZE];
    static uint8_t command_index = 0U;

    uint8_t received_byte = 0U;

    if (!uart_read_byte_non_blocking(&received_byte)) {

        return;
    }

    if (app_uart_demo_is_line_end(received_byte)) {

        command_buffer[command_index] = APP_UART_COMMAND_STRING_TERMINATOR;
        app_uart_demo_process_command(command_buffer);
        command_index = 0U;
        return;
    }

    if (app_uart_demo_is_backspace(received_byte)) {

        if (command_index > 0U) {

            command_index--;
            uart_write_string(APP_UART_RESPONSE_BACKSPACE_SEQUENCE);
        }

        return;
    }

    if (command_index < (APP_UART_COMMAND_BUFFER_SIZE - 1U)) {

        command_buffer[command_index] = (char)received_byte;
        command_index++;
        uart_write_byte(received_byte);
    }
}

static void app_uart_demo_process_command(const char *command) {

    if (strcmp(command, APP_UART_COMMAND_HELP) == 0) {

        uart_write_string(APP_UART_RESPONSE_HELP);
    }

    else if (strcmp(command, APP_UART_COMMAND_STATUS) == 0) {

        uart_write_string(APP_UART_RESPONSE_STATUS);
    }

    else if (strcmp(command, APP_UART_COMMAND_LED_ON) == 0) {

        gpio_led_set(true);
        uart_write_string(APP_UART_RESPONSE_LED_ON);
    }

    else if (strcmp(command, APP_UART_COMMAND_LED_OFF) == 0) {

        gpio_led_set(false);
        uart_write_string(APP_UART_RESPONSE_LED_OFF);
    }

    else if (strcmp(command, APP_UART_COMMAND_BLINK) == 0) {

        for (uint8_t i = 0U; i < APP_UART_BLINK_COUNT; i++) {

            gpio_led_set(true);
            delay_ms(APP_UART_BLINK_DELAY_MS);

            gpio_led_set(false);
            delay_ms(APP_UART_BLINK_DELAY_MS);
        }

        uart_write_string(APP_UART_RESPONSE_BLINK);
    }

    else {

        uart_write_string(APP_UART_RESPONSE_UNKNOWN_COMMAND);
    }
}

static bool app_uart_demo_is_line_end(uint8_t byte) {

    return ((byte == (uint8_t)APP_UART_COMMAND_END_CR) ||
            (byte == (uint8_t)APP_UART_COMMAND_END_LF));
}

static bool app_uart_demo_is_backspace(uint8_t byte) {

    return ((byte == APP_UART_COMMAND_BACKSPACE) || (byte == APP_UART_COMMAND_DELETE));
}
