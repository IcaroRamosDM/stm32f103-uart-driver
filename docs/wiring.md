# Hardware Wiring

## Overview

This project uses USART1 from STM32F103C8T6 for serial communication with a Linux host machine.

Communication is performed through a USB-to-TTL serial adapter.

Examples:

- FTDI
- CH340
- CP2102

The adapter converts:

```text
USB ↔ UART (TTL level)
```

allowing direct terminal interaction using tools such as `picocom`.

---

## USART1 Pins

STM32F103 uses:

| Pin | Function |
|---|---|
| PA9 | USART1 TX |
| PA10 | USART1 RX |

These pins are used for:

- command parser
- UART echo
- serial debugging

---

## USB-TTL Connection

### Required connections

| STM32 | Function | USB-TTL |
|---|---|---|
| PA9 | TX | RX |
| PA10 | RX | TX |
| GND | GND | GND |

Important rule:

```text
TX connects to RX
RX connects to TX
```

Do not connect:

```text
TX → TX
RX → RX
```

This is one of the most common mistakes.

---

## Power Supply

Do not connect:

```text
USB-TTL VCC (red wire)
```

unless voltage compatibility is fully confirmed.

This project should be powered using:

- ST-Link
- board USB
- regulated external supply

Reason:

some USB-TTL adapters provide 5V output, which may damage the MCU or create unstable behavior.

Only GND must be shared.

---

## Typical USB-TTL Cable Colors

Common 4-wire cable example:

| Wire Color | Typical Function |
|---|---|
| Red | VCC |
| Black | GND |
| White | RX |
| Green | TX |

This may vary by manufacturer.

Always verify before connecting.

Do not trust color alone.

---

## Linux Serial Device

After connecting the adapter:

```bash
ls /dev/ttyUSB*
```

Expected result:

```text
/dev/ttyUSB0
```

Some adapters may appear as:

```text
/dev/ttyACM0
```

depending on the chipset and driver.

---

## Linux Serial Permission

If opening the terminal returns:

```text
Permission denied
```

add your user to the `dialout` group:

```bash
sudo usermod -aG dialout $USER
```

Then:

- logout/login

or

- reboot

Check with:

```bash
groups
```

Expected:

```text
dialout
```

This allows serial access without using `sudo`.

---

## Installing picocom

Install using:

```bash
sudo apt update
sudo apt install picocom
```

Alternative tools:

- screen
- minicom
- gtkterm

This project uses `picocom`.

---

## Opening Serial Terminal

Use:

```bash
picocom -b 115200 /dev/ttyUSB0
```

Where:

- baudrate = 115200
- data bits = 8
- parity = none
- stop bits = 1

This is standard:

```text
115200 8N1
```

---

## Exit picocom

Use:

```text
CTRL + A
CTRL + X
```

## Expected Startup Output

After board reset:

```text
UART command parser ready
>
```

If this message does not appear:

- press RESET on the board
- verify TX/RX wiring
- verify baudrate
- verify USB-TTL device path

---

## Common Debugging Cases

### TX works, RX does not

Usually caused by:

- TX/RX swapped incorrectly
- RX wire disconnected
- wrong USART pin

---

### Nothing appears in terminal

Usually caused by:

- wrong `/dev/ttyUSBx`
- missing GND
- baudrate mismatch
- serial permission issue
- board not powered

---

### RX blocking works, interrupt RX does not

Usually caused by:

- wrong vector table position
- IRQ not enabled in NVIC
- RX interrupt not enabled in USART

This occurred during project development and was resolved by correcting the USART1 ISR position in the startup vector table.

---

## Validation Sequence

Recommended test order:

### Step 1

Validate TX only:

```text
STM32 → terminal
```

---

### Step 2

Validate blocking RX echo:

```text
terminal → STM32 → terminal
```

---

### Step 3

Validate interrupt-driven RX

---

### Step 4

Validate command parser

This sequence reduces debugging complexity significantly.
