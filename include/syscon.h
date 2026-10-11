#ifndef _SYSCON_H_
#define _SYSCON_H_

static const unsigned int sys_con_addr = 0x40000000;

// TODO: Should add bitmasks to prevent write to reserved fields of clock controls?
void set_enable_clk_ctrl(unsigned int clk_ctrl_idx, unsigned int value) {
  const unsigned int clk_ctrl_set_offsets[] = { 0x220, 0x224, 0x228, 0x22C };
  volatile unsigned int* const addr = (unsigned int*) (sys_con_addr + clk_ctrl_set_offsets[clk_ctrl_idx]);
  *addr = value;
  return;
}

void set_disable_clk_ctrl(unsigned int clk_ctrl_idx, unsigned int value) {
  const unsigned int clk_ctrl_set_offsets[] = { 0x240, 0x244, 0x248, 0x24C };
  volatile unsigned int* const addr = (unsigned int*) (sys_con_addr + clk_ctrl_set_offsets[clk_ctrl_idx]);
  *addr = value;
  return;
}

const unsigned int syscon_clock_ctrl_offset = 0xA18;

#define SYSCON_CLOCK_CTRL_MASK_A 0x000002FF
#define SYSCON_CLOCK_CTRL_MASK_B 0x00000081
void clock_ctrl_set(uint32_t bitmask) {
	volatile uint32_t* syscon_clock_ctrl = (uint32_t*) (sys_con_addr + syscon_clock_ctrl_offset);
	*syscon_clock_ctrl = (*syscon_clock_ctrl | bitmask | SYSCON_CLOCK_CTRL_MASK_B) & SYSCON_CLOCK_CTRL_MASK_A;
	return;
}

void clock_ctrl_clr(uint32_t bitmask) {
	volatile uint32_t* syscon_clock_ctrl = (uint32_t*) (sys_con_addr + syscon_clock_ctrl_offset);
	*syscon_clock_ctrl = ((*syscon_clock_ctrl & ~bitmask) | SYSCON_CLOCK_CTRL_MASK_B) & SYSCON_CLOCK_CTRL_MASK_A;
	return;
}

#endif //_SYSCON_H_
