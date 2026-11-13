#ifndef _FPU_H_
#define _FPU_H_

static const unsigned int cpacr_addr = 0xE000ED88;

__attribute__((constructor(101))) int init_fpu(void) {
	volatile uint32_t* cpacr = (uint32_t*) (cpacr_addr);
	// Set the privilege level for accessing the FPU
	*cpacr = *cpacr | (0x03U << 22) | (0x03U << 20);
	return 0;
}

#endif //_FPU_H_
