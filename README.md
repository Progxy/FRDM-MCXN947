# FRDM-MCXN947 Bare-Metal Demos

A collection of small **bare-metal demos** for the [FRDM-MCXN947](https://www.nxp.com/design/design-center/development-boards-and-designs/FRDM-MCXN947) development board.

The demos are written to run directly on the **NXP MCXN947** without an operating system.

## Requirements

* FRDM-MCXN947 development board
* `arm-none-eabi-gcc`
* `arm-none-eabi-gdb`
* [LinkServer](https://www.nxp.com/design/design-center/software/MCUXpresso-SDK)
* `make`

## Build

From the demo directory:

```bash
make build/main.elf
```

This produces:

```text
build/main.elf
```

## Flash and Run

The simplest way to flash the binary is:

```bash
LinkServer flash auto load build/main.elf
```

Alternatively, start a LinkServer GDB server:

```bash
LinkServer gdbserver auto
```

Then, in another terminal:

```bash
arm-none-eabi-gdb build/main.elf
```

Connect to the target:

```gdb
target extended-remote localhost:3333
```

Load the program and start execution:

```gdb
load
continue
```

or simply:

```gdb
c
```

## Repository Structure

Each demo is kept small and self-contained, focusing on a particular **MCXN947 peripheral, subsystem, or bare-metal feature**.

```text
.
├── build/
├── include/
├── Makefile
├── linker.ld
├── startup.c
├── main.c
└── README.md
```

The exact structure may vary between demos.
