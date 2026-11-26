#ifndef _NPU_H_
#define _NPU_H_

#include <stdint.h>
#include <syscon.h>
#include <utils.h>
#include <nvic.h>
#include <fpu.h>

__attribute__((constructor(105))) int init_npu(void) {
	set_ahb_clk_ctrl(3, 1U << NPU);
	return 0;
}

#endif //_NPU_H_
