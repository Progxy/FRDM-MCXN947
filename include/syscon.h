#ifndef _SYSCON_H_
#define _SYSCON_H_

#define SYSCON_CLKIN_ENA_FM_USBH_LPT_MASK 0x02
#define SYSCON_CLKIN_ENA_MASK             0x20

static const unsigned int syscon_addr = 0x40000000;

// TODO: Should add bitmasks to prevent write to reserved fields of clock controls?
void set_enable_clk_ctrl(unsigned int clk_ctrl_idx, unsigned int value) {
  const unsigned int clk_ctrl_set_offsets[] = { 0x220, 0x224, 0x228, 0x22C };
  volatile unsigned int* const addr = (unsigned int*) (syscon_addr + clk_ctrl_set_offsets[clk_ctrl_idx]);
  *addr = value;
  return;
}

void set_disable_clk_ctrl(unsigned int clk_ctrl_idx, unsigned int value) {
  const unsigned int clk_ctrl_set_offsets[] = { 0x240, 0x244, 0x248, 0x24C };
  volatile unsigned int* const addr = (unsigned int*) (syscon_addr + clk_ctrl_set_offsets[clk_ctrl_idx]);
  *addr = value;
  return;
}

const unsigned int syscon_clock_ctrl_offset = 0xA18;

#define SYSCON_CLOCK_CTRL_MASK_A 0x000002FF
#define SYSCON_CLOCK_CTRL_MASK_B 0x00000081
void clock_ctrl_set(uint32_t bitmask) {
	volatile uint32_t* syscon_clock_ctrl = (uint32_t*) (syscon_addr + syscon_clock_ctrl_offset);
	*syscon_clock_ctrl = (*syscon_clock_ctrl | bitmask | SYSCON_CLOCK_CTRL_MASK_B) & SYSCON_CLOCK_CTRL_MASK_A;
	return;
}

void clock_ctrl_clr(uint32_t bitmask) {
	volatile uint32_t* syscon_clock_ctrl = (uint32_t*) (syscon_addr + syscon_clock_ctrl_offset);
	*syscon_clock_ctrl = ((*syscon_clock_ctrl & ~bitmask) | SYSCON_CLOCK_CTRL_MASK_B) & SYSCON_CLOCK_CTRL_MASK_A;
	return;
}

const unsigned int syscon_i3c0_clock_sel = 0x530;
const unsigned int syscon_i3c0_clock_div = 0x540;
void syscon_set_i3c0_clock(uint8_t sel, uint8_t div) {
	volatile uint32_t* i3c0_clock_sel = (uint32_t*) (syscon_addr + syscon_i3c0_clock_sel);
	*i3c0_clock_sel = sel & 0x07;
	volatile uint32_t* i3c0_clock_div = (uint32_t*) (syscon_addr + syscon_i3c0_clock_div);
	*i3c0_clock_div = (1U << 30) | div;
	return;
}

#endif //_SYSCON_H_
