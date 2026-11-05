# Contributing

Describe the observable problem, the expected behavior and how to reproduce it.
Keep changes small, use descriptive names and preserve the existing license.

Run make test and make sanitize, then cross-compile with the pinned libopencm3 revision documented in docs/build.md.

Use the checked-in .clang-format for maintained C sources. Keep generated build
products out of source directories.

Report automated results separately from physical-device or live-kernel checks.
Document public API, command, wiring and behavior changes in the relevant guide
and CHANGELOG.md. Never include private captures, credentials or board data.
