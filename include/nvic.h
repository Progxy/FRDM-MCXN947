#ifndef _NVIC_H_
#define _NVIC_H_

#include <stdint.h>

static const unsigned int nvic_iser_addr  = 0xE000E100;
static const unsigned int nvic_icer_addr  = 0xE000E180;
static const unsigned int nvic_shcsr_addr = 0xE000ED24;

typedef enum {
	IRQ_LP_FLEXCOMM0 = 35, IRQ_LP_FLEXCOMM1, IRQ_LP_FLEXCOMM2, IRQ_LP_FLEXCOMM3, IRQ_LP_FLEXCOMM4, IRQ_LP_FLEXCOMM5, IRQ_LP_FLEXCOMM6, IRQ_LP_FLEXCOMM7, IRQ_LP_FLEXCOMM8, IRQ_LP_FLEXCOMM9,
	IRQ_USBHS1_PHY = 66, IRQ_USBHS1,
	IRQ_I3C0 = 95, IRQ_I3C1,
	IRQ_ENET0 = 139, IRQ_ENET0_PM, IRQ_ENET0_LPI
} NVICIRQs;

void nvic_enable_irq(uint32_t irq) {
	volatile uint32_t* nvic_iser = (uint32_t*) (nvic_iser_addr);
	nvic_iser[irq >> 5] = 1U << (irq & 0x1F);
	return;
}

void nvic_disable_irq(uint32_t irq) {
	volatile uint32_t* nvic_icer = (uint32_t*) (nvic_icer_addr);
	nvic_icer[irq >> 5] = 1U << (irq & 0x1F);
	return;
}

__attribute__((constructor(101))) int init_default_irqs(void) {
	volatile uint32_t* nvic_shcsr = (uint32_t*) (nvic_shcsr_addr);
	*nvic_shcsr |= 0x0FU << 16;
	return 0;
}

#endif //_NVIC_H_
