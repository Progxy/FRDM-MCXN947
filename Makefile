CFLAGS  = -mcpu=cortex-m33 -mthumb -mfloat-abi=soft -ffreestanding -fno-builtin -Wall -Wextra -Werror -Og -ggdb -I./include
LDFLAGS = -mcpu=cortex-m33 -mthumb -mfloat-abi=soft -T linker.ld -nostdlib -Wl,--gc-sections
OBJECTS = build/main.o build/startup.o

clean:
	rm -rf build

build:
	mkdir -p build

build/startup.o: startup.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

build/main.o: main.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

build/main.elf: build build/main.o build/startup.o
	arm-none-eabi-gcc $(LDFLAGS) $(OBJECTS) -o $@

build/startup.elf: build
	arm-none-eabi-gcc $(LDFLAGS) startup.s -o $@