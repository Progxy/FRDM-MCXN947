CFLAGS      := -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -ffreestanding -fno-builtin -Wall -Wextra -Werror -Og -ggdb -I../../include
LDFLAGS     := -mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -T ../../include/linker.ld -nostdlib -Wl,--gc-sections
LINK_SERVER := /Applications/LinkServer_26.9.130/LinkServer

build: ../../build/els.elf

clean:
	rm -rf ../../build

../../build/els.elf: main.c ../../build/startup.o
	arm-none-eabi-gcc $(CFLAGS) $(LDFLAGS) $^ -o $@

flash: ../../build/els.elf
	$(LINK_SERVER) flash auto load $<
