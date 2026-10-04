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

void nmi_handler(void)           __attribute__((weak, alias("default_handler"), naked, noreturn));
void hard_fault_handler(void)    __attribute__((weak, alias("default_handler"), naked, noreturn));
void mem_manage_handler(void)    __attribute__((weak, alias("default_handler"), naked, noreturn));
void bus_fault_handler(void)     __attribute__((weak, alias("default_handler"), naked, noreturn));
void usage_fault_handler(void)   __attribute__((weak, alias("default_handler"), naked, noreturn));
void sv_call_handler(void)       __attribute__((weak, alias("default_handler"), naked, noreturn));
void debug_monitor_handler(void) __attribute__((weak, alias("default_handler"), naked, noreturn));
void pend_sv_handler(void)       __attribute__((weak, alias("default_handler"), naked, noreturn));
void sys_tick_handler(void)      __attribute__((weak, alias("default_handler"), naked, noreturn));
void usbhs_dcd_handler(void)     __attribute__((weak, alias("default_handler"), naked, noreturn));

extern const uint32_t vector_table[];

__attribute__((section(".image_header"), used))
const uint32_t vector_table[] = {
    (uint32_t) &_stack, // Initial SP
    (uint32_t) reset_handler, // Initial PC

 	// First 6 Vector Table Entries
    (uint32_t) nmi_handler,
    (uint32_t) hard_fault_handler,
    (uint32_t) mem_manage_handler,
    (uint32_t) bus_fault_handler,
    (uint32_t) usage_fault_handler,
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
    (uint32_t) sv_call_handler,
    (uint32_t) debug_monitor_handler,
    0,
    (uint32_t) pend_sv_handler,
    (uint32_t) sys_tick_handler,

    /* External interrupts 16 - 171 */
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
	(uint32_t) usbhs_dcd_handler,
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
    __asm__ volatile("bx lr");
}
