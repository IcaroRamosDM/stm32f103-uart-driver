#include "drivers/uart/uart_buffer.h"

#include <pthread.h>
#include <sched.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TRANSFER_COUNT 200000U
#define CHECK(condition) check((condition), #condition, __LINE__)

static void check(bool condition, const char *expression, int line) {
    if (!condition) {
        fprintf(stderr, "test_uart_buffer.c:%d: %s\n", line, expression);
        exit(EXIT_FAILURE);
    }
}

static void test_capacity_and_wraparound(void) {
    uart_buffer_t buffer;
    uart_buffer_init(&buffer);
    CHECK(uart_buffer_is_empty(&buffer));
    uint8_t byte = 0xA5U;
    bool result = uart_buffer_pop(&buffer, &byte);
    CHECK(!result && byte == 0xA5U);
    for (unsigned cycle = 0U; cycle < 4U; cycle++) {
        for (unsigned index = 0U; index < UART_BUFFER_SIZE - 1U; index++) {
            result = uart_buffer_push(&buffer, (uint8_t)index);
            CHECK(result);
        }
        CHECK(uart_buffer_is_full(&buffer));
        result = uart_buffer_push(&buffer, 0xFFU);
        CHECK(!result);
        for (unsigned index = 0U; index < UART_BUFFER_SIZE - 1U; index++) {
            result = uart_buffer_pop(&buffer, &byte);
            CHECK(result && byte == (uint8_t)index);
        }
        CHECK(uart_buffer_is_empty(&buffer));
    }
    result = uart_buffer_pop(&buffer, NULL);
    CHECK(!result);
    result = uart_buffer_push(NULL, 0U);
    CHECK(!result);
}

static void *produce(void *context) {
    uart_buffer_t *buffer = context;
    for (unsigned index = 0U; index < TRANSFER_COUNT; index++) {
        while (!uart_buffer_push(buffer, (uint8_t)index)) {
            sched_yield();
        }
    }
    return NULL;
}

static void test_single_producer_single_consumer(void) {
    uart_buffer_t buffer;
    uart_buffer_init(&buffer);
    pthread_t producer;
    int status = pthread_create(&producer, NULL, produce, &buffer);
    CHECK(status == 0);
    for (unsigned index = 0U; index < TRANSFER_COUNT; index++) {
        uint8_t byte;
        while (!uart_buffer_pop(&buffer, &byte)) {
            sched_yield();
        }
        CHECK(byte == (uint8_t)index);
    }
    status = pthread_join(producer, NULL);
    CHECK(status == 0);
    CHECK(uart_buffer_is_empty(&buffer));
}

int main(void) {
    test_capacity_and_wraparound();
    test_single_producer_single_consumer();
    printf("PASS: capacity, overflow, wraparound and %u concurrent transfers\n", TRANSFER_COUNT);
    return EXIT_SUCCESS;
}
