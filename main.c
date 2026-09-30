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
  volatile unsigned int* gpio_pddr_addr = (unsigned int*) gpio_bank_offset[gpio_bank] + pddr_offset;
  *gpio_pddr_addr = (*gpio_pddr_addr | (mode << gpio)) & ~((!mode) << gpio);
}

void set_gpio_state(PinState state, unsigned int gpio_bank, unsigned int gpio) {
  if (gpio_bank > 5 || gpio > 31) return;
  const unsigned int pdor_offset = 0x40;
  volatile unsigned int* gpio_pdor_addr = (unsigned int*) gpio_bank_offset[gpio_bank] + pdor_offset;
  *gpio_pdor_addr = (*gpio_pdor_addr | (state << gpio)) & ~((!state) << gpio);
}

int main(void) {
  	set_gpio_mode(OUTPUT, 0, 27);
 	set_gpio_state(HIGH, 0, 27);

 	volatile int cnt = 0;
 	volatile PinState state = HIGH;
  	while (TRUE) {
   		cnt++;
  		if ((cnt % 1000000) == 0) set_gpio_state(!state, 0, 27);
 	}

  	return 0;
}
