#ifndef _PORT_H_
#define _PORT_H_

#include <stdint.h>

const unsigned int port_addr[] = { 0x40116000, 0x40117000, 0x40118000, 0x40119000, 0x4011A000, 0x40042000 };
const unsigned int pin_base_addr = 0x80;

// TODO: Add masks for each pin, also check if given port-pin combination exists
int set_pin_function(uint8_t port, uint8_t pin, uint32_t val) {
	if (port > 5) return -1;
	else if (pin > 31) return -1;
	volatile uint32_t* pin_reg = (uint32_t*) (port_addr[port] + pin_base_addr + pin * 4);
	if (*pin_reg & (1U << 15)) return -1; // register locked
	*pin_reg = val;
	return 0;
}

#endif //_PORT_H_
