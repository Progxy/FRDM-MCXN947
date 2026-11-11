#ifndef _PORT_H_
#define _PORT_H_

#include <stdint.h>

const unsigned int port_addr[] = { 0x40116000, 0x40117000, 0x40118000, 0x40119000, 0x4011A000, 0x40042000 };
const unsigned int pin_base_addr = 0x80;

typedef enum { PS = 0, PE, PV, SRE, PFE, ODE, DSE, MUX = 8, IBE = 12, INV, LK = 15 } PortPinFields;

typedef enum {
	PS_BITMASK         = (1U  << PS),
	PE_BITMASK         = (1U  << PE),
	PV_BITMASK         = (1U  << PV),
	SRE_BITMASK        = (1U  << SRE),
	PFE_BITMASK        = (1U  << PFE),
	ODE_BITMASK        = (1U  << ODE),
	DSE_BITMASK        = (1U  << DSE),
	MUX_BITMASK        = (0xF << MUX),
	IBE_BITMASK        = (1U  << IBE),
	INV_BITMASK        = (1U  << INV),
	LK_BITMASK         = (1U  << LK),
	RSV_BITMASK        = 0xFFFF4080,
	P17_RSV_BITMASK    = RSV_BITMASK | 0x04,
	P10_15_RSV_BITMASK = RSV_BITMASK | 0x14,
	P18_19_RSV_BITMASK = P10_15_RSV_BITMASK,
	P21_RSV_BITMASK    = P17_RSV_BITMASK,
	P22_31_RSV_BITMASK = P10_15_RSV_BITMASK
} PortPinBitmasks;

uint32_t get_pin_bitmask(uint8_t pin) {
	if (pin >= 10 && pin <= 15) return P10_15_RSV_BITMASK;
	else if (pin == 17) return P17_RSV_BITMASK;
	else if (pin == 18 || pin == 19) return P18_19_RSV_BITMASK;
	else if (pin == 21) return P21_RSV_BITMASK;
	else if (pin >= 22) return P22_31_RSV_BITMASK;
	return RSV_BITMASK;
}

// TODO: Check if given port-pin combination exists
int set_pin_function(uint8_t port, uint32_t pin, uint32_t val) {
	if (port > 5) return -1;
	else if (pin > 31) return -1;
	volatile uint32_t* pin_reg = (uint32_t*) (port_addr[port] + pin_base_addr + pin * 4);
	if (*pin_reg & (1U << LK)) return -1; // register locked
	const uint32_t rsv_bitmask = get_pin_bitmask(pin);
	*pin_reg = val & (~rsv_bitmask);
	return 0;
}

int update_pin_function(uint8_t port, uint32_t pin, uint32_t field_val, uint32_t field_bitmask) {
	if (port > 5) return -1;
	else if (pin > 31) return -1;
	volatile uint32_t* pin_reg = (uint32_t*) (port_addr[port] + pin_base_addr + pin * 4);
	if (*pin_reg & (1U << LK)) return -1; // register locked
	const uint32_t rsv_bitmask = get_pin_bitmask(pin);
	*pin_reg = ((*pin_reg & (~field_bitmask)) | field_val) & (~rsv_bitmask);
	return 0;
}

uint32_t get_pin_function(uint8_t port, uint32_t pin) {
	if (port > 5) return 0;
	else if (pin > 31) return 0;
	volatile uint32_t* pin_reg = (uint32_t*) (port_addr[port] + pin_base_addr + pin * 4);
	return *pin_reg;
}

#endif //_PORT_H_
