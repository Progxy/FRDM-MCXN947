.syntax unified
.cpu cortex-m33
.thumb

.global reset_handler
.global default_handler

.extern _stack
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss

.section .isr_vector, "a", %progbits
.align 8

.word _stack
.word reset_handler
.word default_handler        /* NMI */
.word default_handler        /* HardFault */
.word default_handler        /* MemManage */
.word default_handler        /* BusFault */
.word default_handler        /* UsageFault */
.word 0
.word 0
.word 0
.word 0
.word default_handler        /* SVCall */
.word default_handler        /* DebugMonitor */
.word 0
.word default_handler        /* PendSV */
.word default_handler        /* SysTick */

// External IRQs handlers
.rept 240
    .word default_handler
.endr

.section .text.reset_handler, "ax", %progbits
.thumb_func

reset_handler:
	ldr r0, =0x707
1:
    b 1b

.section .text.default_handler, "ax", %progbits
.thumb_func

default_handler:
    b default_handler
