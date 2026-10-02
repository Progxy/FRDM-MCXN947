ENTRY(reset_handler)

MEMORY {
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x200000
	RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 0x060000
}

# Stack at the end of RAM
_stack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {
    _image_start = .;
    .text : ALIGN(4) {
        FILL(0xff)
		KEEP(*(.image_header))
		. = ALIGN(4);
        *(.after_vectors*)
        *(.text*)
        *(.rodata .rodata.* .constdata .constdata.*)
        . = ALIGN(4);
    } > FLASH

    .data : ALIGN(4) {
        _sdata = .;
        *(.data*)
        *(.ramfunc*)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    _sidata = LOADADDR(.data);
    _image_end = .;

    .bss : ALIGN(4) {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /DISCARD/ : {
        *(.note*)
        *(.comment*)
        *(.eh_frame*)
    }

	_image_length = SIZEOF(.text) + SIZEOF(.data) + SIZEOF(.bss);
}
