MEMORY {
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 2M
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 320K
}

# Stack at the end of RAM
_stack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {
    .isr_vector : {
        . = ALIGN(256);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text : {
        . = ALIGN(4);

        *(.text.Reset_Handler)
        *(.text*)
        *(.rodata*)

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
}
