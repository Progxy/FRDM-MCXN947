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

typedef struct {
	uint8_t mstena: 2;
	uint8_t rsv: 1;
	uint8_t disto: 1;
	uint8_t hkeep: 2;
	uint8_t odstop: 1;
	uint8_t rsv1: 1;
	uint8_t ppbaud: 4;
	uint8_t pplow: 4;
	uint8_t odbaud;
	uint8_t odhpp: 1;
	uint8_t skew: 3;
	uint8_t i2c_baud: 4;
} __attribute__((packed)) mconfig_t;

typedef struct {
	uint8_t rsv;
	uint8_t slv_start: 1;
	uint8_t mctrl_done: 1;
	uint8_t complete: 1;
	uint8_t rx_pend: 1;
	uint8_t tx_not_full: 1;
	uint8_t ibi_won: 1;
	uint8_t rsv1: 1;
	uint8_t err_warn: 1;
	uint8_t rsv2: 3;
	uint8_t now_master: 1;
	uint8_t rsv3: 4;
	uint8_t rsv4;
} __attribute__((packed)) mintset_t;

typedef enum { ON, OFF, CAPABLE, I2C } I3CControllerMode;

static_assert(sizeof(mconfig_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(mintset_t) == 4, "Size must be 4 bytes");

// NOTE: The I3C Registers require 32-bit access both in read and write mode.
void init_i3c(void) {
	// Enable clock with syscon, ensure that the clock runs at 24Mhz
	ClkFreq clk_freq = get_firc_clk_freq();
	uint8_t clk_div = 0x01 + clk_freq * 4;
	syscon_set_i3c0_clock(0x3, clk_div);
	set_enable_clk_ctrl(3, 0);

	// MCONFIG Register:
	volatile mconfig_t* mconfig = (mconfig_t*) (i3c0_addr + mconfig_addr);
	mconfig_t mconfig_ = *mconfig;
	mconfig_.mstena = I2C;
	mconfig_.hkeep = 2;
	mconfig_.ppbaud = 0;  // Each clock period is equal to 1 ppbaud, effectively halve the input clock
	mconfig_.odbaud = 23; // Divide the ppbaud period down so that the odbauds period is 1us
	mconfig_.odhpp = 0;
	mconfig_.i2c_baud = 9; // Divide the odbaud so that each i2baud period is 4us, and add an extra odbaud period to the low signal (so 4us + 1us)
	mconfig_.skew = 0;
	*mconfig = mconfig_;

	// Set up interrupts for Controller Status (MSTATUS) in Controller Interrupt Set (MINTSET).
	// Or set up interrupts for Target Status (SSTATUS) in Target Interrupt Set (SINTSET).

	volatile mintset_t* mintset = (mintset_t*) (i3c0_addr + mint_set_addr);
	mintset_t mintset_ = *mintset;
	mintset_.slv_start = 1;
	mintset_.mctrl_done = 1;
	mintset_.complete = 1;
	mintset_.err_warn = 1;
	*mintset = mintset_;

	return;
}

int init_i2c_tx(const uint8_t addr) {
	(void)addr;
	return I3C_OK;
}

int init_i3c_tx(const uint8_t addr) {
	(void)addr;
	return I3C_OK;
}

int send_i2c(const void* data, const unsigned int size) {
	(void) data;
	(void) size;
	return I3C_OK;
}

int send_i3c(const void* data, const unsigned int size) {
	(void)data;
	(void)size;
	return I3C_OK;
}

int recv_i2c(void* data, const unsigned int size) {
	(void) data;
	(void)size;
	return I3C_OK;
}

int recv_i3c(void* data, const unsigned int size) {
	(void) data;
	(void) size;
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
