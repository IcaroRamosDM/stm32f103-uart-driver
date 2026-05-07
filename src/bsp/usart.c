#include "bsp/usart.h"

#include "bsp/usart_config.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

void usart_setup(void) {

    rcc_periph_clock_enable(BSP_USART_GPIO_RCC);
    rcc_periph_clock_enable(BSP_USART_RCC);

    gpio_set_mode(
            BSP_USART_GPIO_PORT,
            GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
            BSP_USART_TX_PIN
    );

    gpio_set_mode(
            BSP_USART_GPIO_PORT,
            GPIO_MODE_INPUT,
            GPIO_CNF_INPUT_FLOAT,
            BSP_USART_RX_PIN
    );

    usart_set_baudrate(BSP_USART_PORT, BSP_USART_BAUDRATE);
    usart_set_databits(BSP_USART_PORT, BSP_USART_DATA_BITS);
    usart_set_stopbits(BSP_USART_PORT, BSP_USART_STOP_BITS);
    usart_set_mode(BSP_USART_PORT, BSP_USART_MODE);
    usart_set_parity(BSP_USART_PORT, BSP_USART_PARITY);
    usart_set_flow_control(BSP_USART_PORT, BSP_USART_FLOW_CONTROL);

    usart_enable(BSP_USART_PORT);
}

void usart_write_byte(uint8_t data) {
    
    usart_send_blocking(BSP_USART_PORT, data);
}

uint8_t usart_read_byte(void) {

    return (uint8_t)usart_recv_blocking(BSP_USART_PORT);
}
