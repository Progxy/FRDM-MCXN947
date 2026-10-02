#include <stdint.h>
#include <stddef.h>
#include <utils.h>

extern void _stack(void);
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _image_length;

extern void main(void);
void default_handler(void);
void reset_handler(void);

void NMI_Handler(void)        __attribute__((weak, alias("default_handler"), naked, noreturn));
void HardFault_Handler(void)  __attribute__((weak, alias("default_handler"), naked, noreturn));
void MemManage_Handler(void)  __attribute__((weak, alias("default_handler"), naked, noreturn));
void BusFault_Handler(void)   __attribute__((weak, alias("default_handler"), naked, noreturn));
void UsageFault_Handler(void) __attribute__((weak, alias("default_handler"), naked, noreturn));
void SVCall_Handler(void)     __attribute__((weak, alias("default_handler"), naked, noreturn));
void DebugMon_Handler(void)   __attribute__((weak, alias("default_handler"), naked, noreturn));
void PendSV_Handler(void)     __attribute__((weak, alias("default_handler"), naked, noreturn));
void SysTick_Handler(void)    __attribute__((weak, alias("default_handler"), naked, noreturn));

extern const uint32_t vector_table[];

__attribute__((section(".image_header"), used))
const uint32_t vector_table[] = {
    (uint32_t) &_stack, // Initial SP
    (uint32_t) reset_handler, // Initial PC

 	// First 6 Vector Table Entries
    (uint32_t) NMI_Handler,
    (uint32_t) HardFault_Handler,
    (uint32_t) MemManage_Handler,
    (uint32_t) BusFault_Handler,
    (uint32_t) UsageFault_Handler,
    0,

 	(uint32_t) &_image_length,
 	0x10400, // Image Type
 	0x00, // Offset to Extended Header

 	// Next 2 Vector Table Entries
    0,
    0,

 	0x00, // Image Execution Address

 	// Remaining Vector Table Entries
    0,
    (uint32_t) SVCall_Handler,
    (uint32_t) DebugMon_Handler,
    0,
    (uint32_t) PendSV_Handler,
    (uint32_t) SysTick_Handler,

    /* External interrupts */
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
    (uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler,
	(uint32_t) default_handler
};

__attribute__((section(".after_vectors.reset"), naked))
void reset_handler(void) {
    __asm volatile("cpsid i");

 	// Load data and bss sections into RAM
    const unsigned int data_size = (uintptr_t)&_edata - (uintptr_t)&_sdata;
    const unsigned int bss_size  = (uintptr_t)&_ebss  - (uintptr_t)&_sbss;

    memcpy(&_sdata, &_sidata, data_size);
    memset(&_sbss, 0, bss_size);

	__asm volatile("cpsie i");

    main();

 	while (TRUE);
}

__attribute__((section(".after_vectors.default_handler"), naked, noreturn))
void default_handler(void) {
    __asm volatile("bkpt");
    while (TRUE);
}
