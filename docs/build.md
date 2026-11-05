# Build and verification

The firmware targets STM32F103C8 / Cortex-M3. Install an ARM bare-metal GCC
toolchain, GNU Make and Git. ST-Link tools are needed only for flashing.

For a fresh workspace, place this repository next to libopencm3 and prepare
the version used by this project:

~~~sh
git clone https://github.com/libopencm3/libopencm3.git ../libopencm3
git -C ../libopencm3 checkout --detach 87a080c94ce67643216464821c752c1c406c6414
make -C ../libopencm3 TARGETS=stm32/f1
~~~

An existing library installation can be selected with OPENCM3_DIR.
Compilation uses C17 with warnings treated as errors. Object files and header
dependency records stay under build/, so editing a header rebuilds its users.

~~~sh
make all
make bin
make hex
~~~

The ELF, BIN and HEX files are under build/.

Host tests require a native C compiler. They replace hardware I/O with local
fixtures and do not access a board:

~~~sh
make test
make sanitize
~~~

The sanitizer target enables AddressSanitizer and UndefinedBehaviorSanitizer.
For the UART concurrency test, a POSIX threads implementation is also required.
These checks establish software behavior in the host model. Firmware
compilation and host tests do not establish electrical operation.

~~~sh
make flash
~~~

Flashing writes the selected firmware through ST-Link. Use the intended board,
node role, pinout and supply. Neither host-test nor all performs flashing.
The explicit erase and reflash targets erase device flash before writing.
Clean removes generated products under BUILD_DIR.
