#ifndef _GPIO_H_
#define _GPIO_H_

typedef enum {
  INPUT  = 0,
  OUTPUT = 1
} PinMode;

typedef enum {
  LOW  = 0,
  HIGH = 1
} PinState;

static const unsigned int gpio_bank_offset[6] = { 0x40096000, 0x40098000, 0x4009A000, 0x4009C000, 0x4009E000, 0x40040000 };
static const unsigned int sys_con_addr = 0x40000000;

void set_gpio_mode(PinMode mode, unsigned int gpio_bank, unsigned int gpio) {
  if (gpio_bank > 5 || gpio > 31) return;
  const unsigned int pddr_offset = 0x54;
  volatile unsigned int* const gpio_pddr_addr = (unsigned int*) (gpio_bank_offset[gpio_bank] + pddr_offset);
  *gpio_pddr_addr = (*gpio_pddr_addr | (mode << gpio)) & ~((!mode) << gpio);
  return;
}

void set_gpio_state(PinState state, unsigned int gpio_bank, unsigned int gpio) {
  if (gpio_bank > 5 || gpio > 31) return;
  const unsigned int pdor_offset = 0x40;
  volatile unsigned int* const gpio_pdor_addr = (unsigned int*) (gpio_bank_offset[gpio_bank] + pdor_offset);
  *gpio_pdor_addr = (*gpio_pdor_addr | (state << gpio)) & ~((!state) << gpio);
  return;
}

unsigned int get_gpio_pin_data(unsigned int gpio_bank, unsigned int gpio) {
  if (gpio_bank > 5 || gpio > 31) return 0xFFFFFFFF;
  const unsigned int pin_data_offset = 0x60;
  const unsigned int* gpio_pin_data_addr = (unsigned int*) gpio_bank_offset[gpio_bank] + pin_data_offset + gpio;
  return *gpio_pin_data_addr;
}

void set_gpio_pin_data(PinState state, unsigned int gpio_bank, unsigned int gpio) {
  if (gpio_bank > 5 || gpio > 31) return;
  const unsigned int pin_data_offset = 0x60;
  volatile unsigned int* const gpio_pin_data_addr = (unsigned int*) gpio_bank_offset[gpio_bank] + pin_data_offset + gpio;
  *gpio_pin_data_addr = state;
  return;
}

void enable_clk_ctrl(unsigned int clk_ctrl_idx, unsigned int idx) {
  const unsigned int clk_ctrl_set_offsets[] = { 0x220, 0x224, 0x228, 0x22C };
  volatile unsigned int* const addr = (unsigned int*) (sys_con_addr + clk_ctrl_set_offsets[clk_ctrl_idx]);
  *addr |= 1U << idx;
  return;
}

void set_enable_clk_ctrl(unsigned int clk_ctrl_idx, unsigned int value) {
  const unsigned int clk_ctrl_set_offsets[] = { 0x220, 0x224, 0x228, 0x22C };
  volatile unsigned int* const addr = (unsigned int*) (sys_con_addr + clk_ctrl_set_offsets[clk_ctrl_idx]);
  *addr |= value;
  return;
}

void disable_clk_ctrl(unsigned int clk_ctrl_idx, unsigned int idx) {
  const unsigned int clk_ctrl_set_offsets[] = { 0x240, 0x244, 0x248, 0x24C };
  volatile unsigned int* const addr = (unsigned int*) (sys_con_addr + clk_ctrl_set_offsets[clk_ctrl_idx]);
  *addr |= 1U << idx;
  return;
}

void set_disable_clk_ctrl(unsigned int clk_ctrl_idx, unsigned int value) {
  const unsigned int clk_ctrl_set_offsets[] = { 0x240, 0x244, 0x248, 0x24C };
  volatile unsigned int* const addr = (unsigned int*) (sys_con_addr + clk_ctrl_set_offsets[clk_ctrl_idx]);
  *addr = value;
  return;
}

void init_gpio_banks(void) {
	// Enable CLK ctrl for gpio bank 0..4
	set_enable_clk_ctrl(0, (1U << 19) | (1U << 20) | (1U << 21) | (1U << 22) | (1U << 23));
 	return;
}

void init_gpio_bank(unsigned int gpio_bank) {
	set_enable_clk_ctrl(0, (1U << (19 + gpio_bank)));
 	return;
}

#endif //_GPIO_H_
