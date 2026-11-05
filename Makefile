# Firmware products stay under build/; host tests never access a board.
PROJECT = uart_driver
BUILD_DIR ?= build
OPENCM3_DIR ?= ../libopencm3
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size
HOST_CC ?= cc
ST_FLASH ?= st-flash

ARCH_FLAGS = -mcpu=cortex-m3 -mthumb
CFLAGS ?= -std=c17 -O2 -g -Wall -Wextra -Wpedantic -Werror
CFLAGS += -ffunction-sections -fdata-sections
CPPFLAGS += -DSTM32F1 -Iinclude -I$(OPENCM3_DIR)/include
LDFLAGS += -Tlinker/stm32f103c8.ld -nostartfiles -Wl,--gc-sections
LDLIBS += -L$(OPENCM3_DIR)/lib -lopencm3_stm32f1
NODE_DIRECTORY =

OUTPUT_DIR = $(BUILD_DIR)$(NODE_DIRECTORY)
OBJECT_DIR = $(OUTPUT_DIR)/objects
TARGET = $(OUTPUT_DIR)/$(PROJECT)
OPENCM3_LIB = $(OPENCM3_DIR)/lib/libopencm3_stm32f1.a
C_SOURCES = $(sort $(shell find src -name '*.c'))
ASM_SOURCES = startup/startup_stm32f103c8.s
OBJECTS = $(patsubst %.c,$(OBJECT_DIR)/%.o,$(C_SOURCES))
OBJECTS += $(patsubst %.s,$(OBJECT_DIR)/%.o,$(ASM_SOURCES))

.PHONY: all bin hex flash erase reflash clean test host-test sanitize
all: $(TARGET).elf

$(TARGET).elf: $(OBJECTS) linker/stm32f103c8.ld $(OPENCM3_LIB)
	@mkdir -p $(dir $@)
	$(CC) $(ARCH_FLAGS) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@
	$(SIZE) $@

$(OBJECT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(ARCH_FLAGS) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJECT_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(ARCH_FLAGS) -c $< -o $@

bin: $(TARGET).bin
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

hex: $(TARGET).hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

flash: bin
	$(ST_FLASH) write $(TARGET).bin 0x08000000

erase:
	$(ST_FLASH) erase

reflash:
	$(MAKE) erase
	$(MAKE) flash

HOST_SANITIZERS ?=
HOST_FLAGS = -std=c17 -O1 -g -Wall -Wextra -Wpedantic -Werror -pthread
ifneq ($(HOST_SANITIZERS),)
HOST_FLAGS += -fsanitize=$(HOST_SANITIZERS) -fno-omit-frame-pointer
endif
HOST_DIRECTORY = $(BUILD_DIR)/host$(if $(HOST_SANITIZERS),-sanitized)
HOST_SOURCES = tests/test_uart_buffer.c src/drivers/uart/uart_buffer.c

test: host-test
host-test:
	@mkdir -p $(HOST_DIRECTORY)
	$(HOST_CC) $(HOST_FLAGS) -Iinclude $(HOST_SOURCES) -o $(HOST_DIRECTORY)/test
	./$(HOST_DIRECTORY)/test
	$(HOST_CC) $(HOST_FLAGS) -Itests/mocks -Iinclude tests/test_uart_driver.c src/drivers/uart/uart.c src/drivers/uart/uart_buffer.c -o $(HOST_DIRECTORY)/driver-test
	./$(HOST_DIRECTORY)/driver-test

sanitize:
	$(MAKE) HOST_SANITIZERS=address,undefined host-test

clean:
	rm -rf -- $(BUILD_DIR)

-include $(OBJECTS:.o=.d)
