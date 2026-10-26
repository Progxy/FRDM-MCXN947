#ifndef _I3C_H_
#define _I3C_H_

#include <stdint.h>
#include "./utils.h"

typedef enum { I3C_OK = 0, I3C_FAIL = 1, I3C_INVALID_BUFFER = 2 } I3CErrors;

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
const unsigned int mwdatab_addr    = 0xB0;
const unsigned int mwdatabe_addr   = 0xB4;
const unsigned int mrdatab_addr    = 0xC0;

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

typedef struct {
	uint8_t request: 2;
	uint8_t rsv: 1;
	uint8_t type: 2;
	uint8_t ibi_resp: 2;
	uint8_t dir: 1;
	uint8_t addr: 7;
	uint8_t rd_term;
	uint8_t rsv1;
} __attribute__((packed)) mctrl_t;

typedef struct {
	uint8_t state: 3;
	uint8_t rsv: 1;
	uint8_t between: 1;
	uint8_t nacked: 1;
	uint8_t ibi_type: 2;
	uint8_t slv_start: 1;
	uint8_t mctrl_done: 1;
	uint8_t complete: 1;
	uint8_t rxpend: 1;
	uint8_t tx_not_full: 1;
	uint8_t ibi_won: 1;
	uint8_t rsv1: 1;
	uint8_t err_warn: 1;
	uint8_t rsv2: 3;
	uint8_t now_master: 1;
	uint8_t rsv3: 4;
	uint8_t ibi_addr: 7;
	uint8_t rsv4: 1;
} __attribute__((packed)) mstatus_t;

typedef enum { ON, OFF, CAPABLE, I2C } I3CControllerMode;
typedef enum { I3C_REQ = 0, I2C_REQ = 1, DDR_REQ = 2 } I3CRequestType;
typedef enum { I3C_READ = 0, I3C_WRITE = 1 } I3CDirection;
typedef enum { ACK = 0, NACK = 1, ACK_MB = 2, MANUAL = 3 } I3CIbiResponse;

static_assert(sizeof(mconfig_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(mintset_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(mstatus_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(mctrl_t)   == 4, "Size must be 4 bytes");

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
	mconfig_.odstop = 1;
	mconfig_.odhpp = 0;
	mconfig_.i2c_baud = 9; // Divide the odbaud so that each i2baud period is 4us, and add an extra odbaud period to the low signal (so 4us + 1us)
	mconfig_.skew = 0;
	*mconfig = mconfig_;

	// Set up interrupts for Controller Status (MSTATUS) in Controller Interrupt Set (MINTSET).
	// Or set up interrupts for Target Status (SSTATUS) in Target Interrupt Set (SINTSET).

	volatile mintset_t* mintset = (mintset_t*) (i3c0_addr + mint_set_addr);
	mintset_t mintset_ = *mintset;
	mintset_.slv_start = 1;
	mintset_.ibi_won = 1;
	mintset_.err_warn = 1;
	// NOTE: rather than handling read/writes using interrupt/asynchronously, we may rather
	//       use a polling/synchronous approach
	// mintset_.rx_pend = 1;
	// mintset_.tx_not_full = 1;
	//mintset_.mctrl_done = 1;
	//mintset_.complete = 1;
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

int send_i2c(const uint8_t addr, const void* data, const unsigned int size, const uint8_t emit_stop) {
	if (data == NULL || size == 0) return -I3C_INVALID_BUFFER;

	volatile mctrl_t* mctrl = (mctrl_t*) (i3c0_addr + mctrl_addr);
	mctrl_t mctrl_ = *mctrl;
	mctrl_.request = 1;
	mctrl_.type = I2C_REQ;
	mctrl_.ibi_resp = 0;
	mctrl_.dir = I3C_WRITE;
	mctrl_.addr = addr;
	mctrl_.rd_term = 0;
	*mctrl = mctrl_;

	volatile uint32_t* mwdatab  = (uint32_t*)  (i3c0_addr + mwdatab_addr);
	volatile uint32_t* mwdatabe = (uint32_t*)  (i3c0_addr + mwdatabe_addr);
	volatile mstatus_t* mstatus = (mstatus_t*) (i3c0_addr + mstatus_addr);
	for (unsigned int i = 0; i < size - 1; ++i) {
		while (!mstatus -> tx_not_full) delay_us(1);
		*mwdatab = ((uint8_t*) data)[i];
	}

	while (!mstatus -> tx_not_full) delay_us(1);
	*mwdatabe = ((uint8_t*) data)[size - 1];

	while (!mstatus -> complete) delay_us(1);

	if (emit_stop) mctrl_.request = 2;
	else           mctrl_.request = 1;
	*mctrl = mctrl_;

	if (emit_stop) {
		while (!mstatus -> mctrl_done) delay_us(1);
	}

	return I3C_OK;
}

int send_i3c(const void* data, const unsigned int size) {
	(void)data;
	(void)size;
	return I3C_OK;
}

int recv_i2c(const uint8_t addr, void* data, const unsigned int size, const uint8_t emit_stop) {
	if (data == NULL || size == 0) return -I3C_INVALID_BUFFER;

	volatile mctrl_t* mctrl = (mctrl_t*) (i3c0_addr + mctrl_addr);
	mctrl_t mctrl_ = *mctrl;
	mctrl_.request = 1;
	mctrl_.type = I2C_REQ;
	mctrl_.ibi_resp = 0;
	mctrl_.dir = I3C_READ;
	mctrl_.addr = addr;
	mctrl_.rd_term = 0;
	*mctrl = mctrl_;

	volatile uint32_t* mrdatab  = (uint32_t*)  (i3c0_addr + mrdatab_addr);
	volatile mstatus_t* mstatus = (mstatus_t*) (i3c0_addr + mstatus_addr);
	for (unsigned int i = 0; i < size; ++i) {
		while (!mstatus -> rxpend) delay_us(1);
		((uint8_t*) data)[i] = *mrdatab;
	}

	while (!mstatus -> complete) delay_us(1);

	if (emit_stop) mctrl_.request = 2;
	else           mctrl_.request = 1;
	*mctrl = mctrl_;

	if (emit_stop) {
		while (!mstatus -> mctrl_done) delay_us(1);
	}

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
