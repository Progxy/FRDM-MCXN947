#ifndef _I3C_H_
#define _I3C_H_

#include <stdint.h>
#include "./utils.h"
#include <clock.h>

typedef enum { I3C_OK = 0, I3C_FAIL = 1, I3C_INVALID_BUFFER = 2, I3C_SLAVE_NACK = 3 } I3CErrors;
typedef enum { I3C0 = 0, I3C1 = 1 } I3CInterface;

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

typedef struct {
	uint8_t rsv: 2;
	uint8_t nack: 1;
	uint8_t wrabt: 1;
	uint8_t rsv1: 4;
	uint8_t rsv2: 1;
	uint8_t hpar: 1;
	uint8_t hcrc: 1;
	uint8_t rsv3: 5;
	uint8_t oread: 1;
	uint8_t owrite: 1;
	uint8_t msg_err: 1;
	uint8_t inv_req: 1;
	uint8_t timeout: 1;
	uint8_t rsv4: 3;
	uint8_t rsv5;
} __attribute__((packed)) merrwarn_t;

typedef enum { I3C_ON, I3C_OFF, I3C_CAPABLE, I2C } I3CControllerMode;
typedef enum { I3C_REQ = 0, I2C_REQ = 1, I3C_DDR_REQ = 2 } I3CRequestType;
typedef enum { I3C_READ = 0, I3C_WRITE = 1 } I3CDirection;
typedef enum { I3C_ACK = 0, I3C_NACK = 1, I3C_ACK_MB = 2, I3C_MANUAL = 3 } I3CIbiResponse;
typedef enum { I3C_NONE = 0, I3C_EMIT_START_ADDR = 1, I3C_EMIT_STOP = 2, I3C_IBI_ACK_NACK = 3, I3C_PROCESS_DAA = 4, I3C_FORCE_EXIT_AND_TARGET_RESET = 6, I3C_AUTO_IBI = 7 } I3CRequest;

static_assert(sizeof(mconfig_t)  == 4, "Size must be 4 bytes");
static_assert(sizeof(mintset_t)  == 4, "Size must be 4 bytes");
static_assert(sizeof(mstatus_t)  == 4, "Size must be 4 bytes");
static_assert(sizeof(mctrl_t)    == 4, "Size must be 4 bytes");
static_assert(sizeof(merrwarn_t) == 4, "Size must be 4 bytes");

// NOTE: The I3C Registers require 32-bit access both in read and write mode.
void init_i3c(void) {
	// Enable clock with syscon, ensure that the clock runs at 24Mhz
	ClkFreq clk_freq = get_firc_clk_freq();
	uint8_t clk_div = 0x01 + clk_freq * 4;
	set_i3c0_clock(0x3, clk_div);
	set_i3c1_clock(0x3, clk_div);
	set_ahb_clk_ctrl(3, (1U << I3C_0) | (1U << I3C_1));
	// Enable P1_16 P1_17 with alternative function 10, also enable port 1 clock?

	mconfig_t mconfig_ = {0};
	mconfig_.mstena = I2C;
	mconfig_.hkeep = 2;
	mconfig_.ppbaud = 0;  // Each clock period is equal to 1 ppbaud, effectively halve the input clock
	mconfig_.odbaud = 23; // Divide the ppbaud period down so that the odbauds period is 1us
	mconfig_.odstop = 1;
	mconfig_.odhpp = 0;
	mconfig_.i2c_baud = 9; // Divide the odbaud so that each i2baud period is 4us, and add an extra odbaud period to the low signal (so 4us + 1us)
	mconfig_.skew = 0;

	volatile mconfig_t* mconfig_i3c0 = (mconfig_t*) (i3c0_addr + mconfig_addr);
	volatile mconfig_t* mconfig_i3c1 = (mconfig_t*) (i3c1_addr + mconfig_addr);
	*mconfig_i3c0 = mconfig_;
	*mconfig_i3c1 = mconfig_;

	mintset_t mintset_ = {0};
	mintset_.slv_start = 1;
	mintset_.ibi_won = 1;
	mintset_.err_warn = 1;
	// NOTE: rather than handling read/writes using interrupt/asynchronously, we may rather
	//       use a polling/synchronous approach
	mintset_.rx_pend = 1;
	mintset_.tx_not_full = 1;
	mintset_.mctrl_done = 1;
	mintset_.complete = 1;

	volatile mintset_t* mintset_i3c0 = (mintset_t*) (i3c0_addr + mint_set_addr);
	volatile mintset_t* mintset_i3c1 = (mintset_t*) (i3c1_addr + mint_set_addr);
	*mintset_i3c0 = mintset_;
	*mintset_i3c1 = mintset_;

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

#define send_i2c(addr, data, size, emit_stop) __send_i2c(I3C0, addr, data, size, emit_stop)
int __send_i2c(const I3CInterface interface, const uint8_t addr, const void* data, const unsigned int size, const uint8_t emit_stop) {
	if (data == NULL || size == 0) return -I3C_INVALID_BUFFER;

	const unsigned int i3c_addr = i3c0_addr + interface * 0x1000;
	volatile mctrl_t* mctrl = (mctrl_t*) (i3c_addr + mctrl_addr);
	mctrl_t mctrl_ = *mctrl;
	mctrl_.request = I3C_EMIT_START_ADDR;
	mctrl_.type = I2C_REQ;
	mctrl_.ibi_resp = I3C_ACK;
	mctrl_.dir = I3C_WRITE;
	mctrl_.addr = addr;
	mctrl_.rd_term = size;
	*mctrl = mctrl_;

	volatile uint32_t* mwdatab  = (uint32_t*)  (i3c_addr + mwdatab_addr);
	volatile uint32_t* mwdatabe = (uint32_t*)  (i3c_addr + mwdatabe_addr);
	volatile mstatus_t* mstatus = (mstatus_t*) (i3c_addr + mstatus_addr);
	for (unsigned int i = 0; i < size - 1; ++i) {
		while (!mstatus -> tx_not_full) delay_us(1);
		*mwdatab = ((uint8_t*) data)[i];
	}

	while (!mstatus -> tx_not_full) delay_us(1);
	*mwdatabe = ((uint8_t*) data)[size - 1];

	mstatus_t st = *mstatus;
	while (!(mstatus -> complete || mstatus -> err_warn)) delay_us(1);
	if (mstatus -> err_warn) {
		merrwarn_t* merrwarn = (merrwarn_t*) (i3c_addr + merr_warn_addr);
		if (merrwarn -> nack) return -I3C_SLAVE_NACK;
		return -I3C_FAIL;
	}

	if (emit_stop) mctrl_.request = I3C_EMIT_STOP;
	else           mctrl_.request = I3C_EMIT_START_ADDR;
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

#define recv_i2c(addr, data, size, emit_stop) __recv_i2c(I3C0, addr, data, size, emit_stop)
int __recv_i2c(const I3CInterface interface, const uint8_t addr, void* data, const unsigned int size, const uint8_t emit_stop) {
	if (data == NULL || size == 0) return -I3C_INVALID_BUFFER;

	const unsigned int i3c_addr = i3c0_addr + interface * 0x1000;
	volatile mctrl_t* mctrl = (mctrl_t*) (i3c_addr + mctrl_addr);
	mctrl_t mctrl_ = *mctrl;
	mctrl_.request = I3C_EMIT_START_ADDR;
	mctrl_.type = I2C_REQ;
	mctrl_.ibi_resp = 0;
	mctrl_.dir = I3C_READ;
	mctrl_.addr = addr;
	mctrl_.rd_term = size + 1;
	*mctrl = mctrl_;

	volatile uint32_t* mrdatab  = (uint32_t*)  (i3c_addr + mrdatab_addr);
	volatile mstatus_t* mstatus = (mstatus_t*) (i3c_addr + mstatus_addr);
	for (unsigned int i = 0; i < size; ++i) {
		while (!mstatus -> rxpend) delay_us(1);
		((uint8_t*) data)[i] = *mrdatab;
	}

	while (!mstatus -> complete) delay_us(1);

	if (emit_stop) mctrl_.request = I3C_EMIT_STOP;
	else           mctrl_.request = I3C_EMIT_START_ADDR;
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
