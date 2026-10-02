#include <stddef.h>
#include <utils.h>
#include <gpio.h>

void main(void) {
    init_gpio_bank(0); // Either initialize one GPIO bank
    init_gpio_banks(); // Or all the GPIO banks

	set_gpio_mode(OUTPUT, 0, 27);
    set_gpio_mode(OUTPUT, 4, 1);

 	volatile PinState state = LOW;
  	while (TRUE) {
		state = !state;
   	 	set_gpio_state(state, 0, 27);
  		set_gpio_state(state, 4, 1);
  		delay(1000);
 	}

  	return;
}
