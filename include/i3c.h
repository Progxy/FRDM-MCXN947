#ifndef _I3C_H_
#define _I3C_H_

#include <stdint.h>
#include "./utils.h"

typedef enum { I3C_OK = 0, I3C_FAIL = 1 } I3CErrors;

const unsigned int i3c0_addr = 0x40021000;
const unsigned int i3c1_addr = 0x40022000;

const unsigned int mconfig_addr    = 0x00;
const unsigned int sconfig_addr    = 0x04;
const unsigned int sstatus_addr    = 0x08;
const unsigned int sctrl_addr      = 0x0C;
const unsigned int mctrl_addr      = 0x84;
const unsigned int mstatus_addr    = 0x88;
const unsigned int mibi_rules_addr = 0x8C;
const unsigned int mint_set_addr   = 0x90;
const unsigned int merr_warn_addr  = 0x9C;
const unsigned int mdma_ctrl_addr  = 0xA0;
const unsigned int mdata_ctrl_addr = 0xAC;

// NOTE: The I3C Registers require 32-bit access both in read and write mode.
void init_i3c(void) {
	// Enable clock with syscon
	// MCONFIG Register:
	// -> MSTENA set to Controller or Target Mode
	// -> HKEEP decided how to implement the pull up
	// -> PPBAUD set the push-pull frequency as a div of FCLK (equivalent to the half-clock period of SCL)
	// -> PPLOW sets the duty cycle for push-pull, indicating how many FCLK cycles to use for low
	// -> OBDAUD sets the number of PPBAUD periods contained within one I3C Open-drain half clock period
	// -> ODHPP enables longer low signal and shorter high signal in Open-drain push-pull I3C configuration
	// -> I2CBAUD indicates the number of ODBAUD periods required to communicate with I2C devices
	// -> SKEW adds additional delay for handling SDA skew from SCL
	// Set up interrupts for Controller Status (MSTATUS) in Controller Interrupt Set (MINTSET).
	// Or set up interrupts for Target Status (SSTATUS) in Target Interrupt Set (SINTSET).
	return;
}

int init_i2c_tx(const uint8_t addr) {
	return I3C_OK;
}

int init_i3c_tx(const uint8_t addr) {
	return I3C_OK;
}

int send_i2c(const void* data, const unsigned int size) {
	return I3C_OK;
}

int send_i3c(const void* data, const unsigned int size) {
	return I3C_OK;
}

int recv_i2c(void* data, const unsigned int size) {
	return I3C_OK;
}

int recv_i3c(void* data, const unsigned int size) {
	return I3C_OK;
}

__attribute__((section(".after_vectors.i3c0_handler"), naked, noreturn))
void i3c0_handler(void) {
	__asm__ volatile("bkpt");
	__asm__ volatile("bx lr");
}

__attribute__((section(".after_vectors.i3c1_handler"), naked, noreturn))
void i3c1_handler(void) {
	__asm__ volatile("bkpt");
	__asm__ volatile("bx lr");
}

#endif //_I3C_H_
