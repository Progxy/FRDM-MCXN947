#include <stdint.h>
#include <stddef.h>
#include <utils.h>

extern uint32_t _stack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern int main(void);
void default_handler(void);
void reset_handler(void);

void NMI_Handler(void)        __attribute__((weak, alias("default_handler"), noreturn));
void HardFault_Handler(void)  __attribute__((weak, alias("default_handler"), noreturn));
void MemManage_Handler(void)  __attribute__((weak, alias("default_handler"), noreturn));
void BusFault_Handler(void)   __attribute__((weak, alias("default_handler"), noreturn));
void UsageFault_Handler(void) __attribute__((weak, alias("default_handler"), noreturn));
void SVCall_Handler(void)     __attribute__((weak, alias("default_handler"), noreturn));
void DebugMon_Handler(void)   __attribute__((weak, alias("default_handler"), noreturn));
void PendSV_Handler(void)     __attribute__((weak, alias("default_handler"), noreturn));
void SysTick_Handler(void)    __attribute__((weak, alias("default_handler"), noreturn));

__attribute__((section(".isr_vector"), used))
const uintptr_t vector_table[] = {
    (uintptr_t)&_stack,
    (uintptr_t)reset_handler,
    (uintptr_t)NMI_Handler,
    (uintptr_t)HardFault_Handler,
    (uintptr_t)MemManage_Handler,
    (uintptr_t)BusFault_Handler,
    (uintptr_t)UsageFault_Handler,
    0,
    0,
    0,
    0,
    (uintptr_t)SVCall_Handler,
    (uintptr_t)DebugMon_Handler,
    0,
    (uintptr_t)PendSV_Handler,
    (uintptr_t)SysTick_Handler,
    /* External interrupts */
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
    (uintptr_t)default_handler,
};

__attribute__((section(".text.reset_handler"), used))
void reset_handler(void) {
    const uintptr_t data_size = (uintptr_t)&_edata - (uintptr_t)&_sdata;
    const uintptr_t bss_size  = (uintptr_t)&_ebss  - (uintptr_t)&_sbss;

    memcpy(&_sdata, &_sidata, data_size);
    memset(&_sbss, 0, bss_size);

    /* SCB->VTOR */
    // *(volatile uint32_t *)0xE000ED08 = (uint32_t)(uintptr_t)vector_table;

    main();

 	while (TRUE);
}

__attribute__((section(".text.default_handler"), noreturn))
void default_handler(void) {
    while (TRUE);
}
