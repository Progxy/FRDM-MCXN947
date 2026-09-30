#include <stddef.h>
#include <utils.h>

typedef enum {
  INPUT  = 0,
  OUTPUT = 1
} PinMode;

typedef enum {
  LOW  = 0,
  HIGH = 1
} PinState;

static const unsigned int gpio_bank_offset[6] = { 0x40096000, 0x40098000, 0x4009A000, 0x4009C000, 0x4009E000, 0x40040000 };

void set_gpio_mode(PinMode mode, unsigned int gpio_bank, unsigned int gpio) {
  if (gpio_bank > 5 || gpio > 31) return;

  if (gpio_bank == 0 && gpio == 10) {
 	const unsigned int offset = 0xA8;
 	volatile unsigned int* const pcr_addr = (unsigned int*) (0x40116000 + offset);
    *pcr_addr &= ~(0xF00);
  }

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

int main(void) {
	volatile unsigned int* const addr = (unsigned int*) (0x40000000 + 0x220);
	*addr |= (1U << 19) | (1U << 13);

 	set_gpio_state(LOW, 0, 27);
	set_gpio_mode(OUTPUT, 0, 27);
 	set_gpio_state(HIGH, 0, 10);
    set_gpio_mode(OUTPUT, 0, 10);
 	set_gpio_state(HIGH, 1, 2);
	set_gpio_mode(OUTPUT, 1, 2);

 	volatile PinState state[] = { LOW, HIGH, HIGH };
 	volatile unsigned int cnt = 0;
  	while (TRUE) {
		state[cnt] = !state[cnt];
  		cnt = (cnt + 1) % 3;

   	 	set_gpio_state(state[0], 0, 27);
        set_gpio_state(state[1], 0, 10);
        set_gpio_state(state[2], 1, 2);

  		delay(1000);
 	}

  	return 0;
}
