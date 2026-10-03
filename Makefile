CFLAGS      := -mcpu=cortex-m33 -mthumb -mfloat-abi=soft -ffreestanding -fno-builtin -Wall -Wextra -Werror -Wno-unused-variable -Og -ggdb -I./include
LDFLAGS     := -mcpu=cortex-m33 -mthumb -mfloat-abi=soft -T ./include/linker.ld -nostdlib -Wl,--gc-sections
LINK_SERVER := /Applications/LinkServer_26.9.130/LinkServer
DEMO_DIRS   := $(wildcard demos/*)

.PHONY: demos

demos: $(DEMO_DIRS)

build: build/main.elf

clean:
	rm -rf build

build/startup.o: include/startup.c
	mkdir -p build
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

$(DEMO_DIRS): build/startup.o
	$(MAKE) -C $@

flash: build/main.elf
	$(LINK_SERVER) flash auto load $<

build/main.elf: main.c build/startup.o
	arm-none-eabi-gcc $(CFLAGS) $(LDFLAGS) $^ -o $@
