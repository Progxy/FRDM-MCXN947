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
  const unsigned int pddr_offset = 0x54;
  volatile unsigned int* const gpio_pddr_addr = (unsigned int*) (gpio_bank_offset[gpio_bank] + pddr_offset);
  *gpio_pddr_addr = (*gpio_pddr_addr | (mode << gpio)) & ~((!mode) << gpio);
}

void set_gpio_state(PinState state, unsigned int gpio_bank, unsigned int gpio) {
  if (gpio_bank > 5 || gpio > 31) return;
  const unsigned int pdor_offset = 0x40;
  volatile unsigned int* const gpio_pdor_addr = (unsigned int*) (gpio_bank_offset[gpio_bank] + pdor_offset);
  *gpio_pdor_addr = (*gpio_pdor_addr | (state << gpio)) & ~((!state) << gpio);
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
    set_gpio_state(LOW, 4, 1);
	set_gpio_mode(OUTPUT, 4, 1);
 	unsigned int pin_data = get_gpio_pin_data(4, 1);
    if (pin_data == 1) {
	   __asm volatile("bkpt");
    }

 	set_gpio_state(HIGH, 0, 27);
	set_gpio_mode(OUTPUT, 0, 27);
 	set_gpio_state(HIGH, 0, 10);
    set_gpio_mode(OUTPUT, 0, 10);
	set_gpio_mode(OUTPUT, 1, 2);
 	set_gpio_state(HIGH, 1, 2);

	__asm volatile("ldr r0, =0x707");
 	__asm volatile("bkpt");

 	volatile int cnt = 0;
 	volatile PinState state = HIGH;
  	while (TRUE) {
   		cnt++;
  		if ((cnt % 1000000) == 0) set_gpio_state(!state, 0, 27);
 	}

  	return 0;
}
