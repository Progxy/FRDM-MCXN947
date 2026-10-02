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

From the main directory or from any of the demos' subfolders:

```bash
make build
```

This produces the `.elf` image to flash into the board.

Furthermore to build all the demos:

```bash
make demos
```

## Flash and Run

The simplest way to flash the binary is:

```bash
make flash
```

Either from the main directory or from any of the demos' subfolders.

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
├── demos/
├── Makefile
├── main.c
└── README.md
```
