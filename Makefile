########################################
# Project
########################################

PROJECT = uart_driver

########################################
# Directories
########################################

BUILD_DIR = build

SRC_DIR = src
INC_DIR = include
STARTUP_DIR = startup
LINKER_DIR = linker

########################################
# Toolchain
########################################

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

########################################
# MCU
########################################

MCU = cortex-m3

########################################
# libopencm3
########################################

OPENCM3_DIR = ../libopencm3

########################################
# Flags
########################################

CFLAGS = \
  -mcpu=$(MCU) \
  -mthumb \
  -DSTM32F1 \
  -Wall \
  -Wextra \
  -Werror \
  -O2 \
  -ffunction-sections \
  -fdata-sections

LDFLAGS = \
  -T$(LINKER_DIR)/stm32f103c8.ld \
  -nostartfiles \
  -Wl,--gc-sections \
  -L$(OPENCM3_DIR)/lib \
  -lopencm3_stm32f1

########################################
# Includes
########################################

INCLUDES = \
  -I$(INC_DIR) \
  -I$(INC_DIR)/app \
  -I$(INC_DIR)/bsp \
  -I$(INC_DIR)/drivers/uart \
  -I$(OPENCM3_DIR)/include

########################################
# Sources
########################################

C_SOURCES = $(shell find $(SRC_DIR) -name "*.c")
ASM_SOURCES = $(STARTUP_DIR)/startup_stm32f103c8.s

########################################
# Objects
########################################

OBJECTS = $(C_SOURCES:.c=.o)
OBJECTS += $(ASM_SOURCES:.s=.o)

########################################
# Build
########################################

all: $(BUILD_DIR)/$(PROJECT).elf

$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

########################################
# Compilation rules
########################################

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

########################################
# Output formats
########################################

bin: all
	$(OBJCOPY) -O binary $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin

hex: all
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex

########################################
# Flash
########################################

flash: bin
	st-flash write $(BUILD_DIR)/$(PROJECT).bin 0x08000000

########################################
# Erase
########################################

erase:
	st-flash erase

reflash: erase flash

########################################
# Clean
########################################

clean:
	rm -rf $(BUILD_DIR)
	find . -name "*.o" -delete

.PHONY: all bin hex flash erase reflash clean
