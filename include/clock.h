#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>
#include "./utils.h"
#include "./syscon.h"

typedef struct {
	uint8_t range: 1;
	uint32_t rsv: 31;
} __attribute__((packed)) scg_firc_cfg_t;

typedef struct {
	uint8_t firc_en: 1;
	uint8_t firc_sten: 1;
	uint8_t rsv: 2;
	uint8_t firc_sclk_periph_en: 1;
	uint8_t firc_fclk_periph_en: 1;
	uint8_t rsv1: 2;
	uint8_t firc_tren: 1;
	uint8_t firc_trup: 1;
	uint8_t trim_lock: 1;
	uint8_t coarse_trim_bypass: 1;
	uint16_t rsv2: 11;
	uint8_t lk: 1;
	uint8_t firc_vld: 1;
	uint8_t firc_sel: 1;
	uint8_t firc_err: 1;
	uint8_t fircerr_i: 1;
	uint8_t rsv3: 2;
	uint8_t firc_acc_ie: 1;
	uint8_t firc_acc: 1;
} __attribute__((packed)) scg_firc_csr_t;


typedef struct {
	uint8_t enable: 1;
	uint8_t sosc_sten: 1;
	uint16_t rsv: 14;
	uint8_t sosc_cm: 1;
	uint8_t sosc_cmre: 1;
	uint8_t rsv1: 5;
	uint8_t lock: 1;
	uint8_t sosc_vld: 1;
	uint8_t sosc_sel: 1;
	uint8_t sosc_err: 1;
	uint8_t rsv2: 3;
	uint8_t sosc_vld_ie: 1;
	uint8_t rsv3: 1;
} __attribute__((packed)) scg_sosc_csr_t;

typedef struct {
	uint8_t rsv: 2;
	uint8_t erefs: 1;
	uint8_t rsv1: 1;
	uint8_t range: 2;
	uint32_t rsv2: 26;
} __attribute__((packed)) scg_sosc_cfg_t;

typedef struct {
	uint8_t ldo_en: 1;
	uint8_t vout_sel: 3;
	uint8_t ldo_bypass: 1;
	uint32_t rsv: 26;
	uint8_t vout_ok: 1;
} __attribute__((packed)) scg_ldo_csr_t;

typedef struct {
	uint32_t rsv: 24;
	uint8_t scs: 4;
	uint8_t rsv1: 4;
} __attribute__((packed)) scg_rccr_t;

typedef scg_rccr_t scg_csr_t;

static_assert(sizeof(scg_csr_t) == 4,      "Size must be 4 bytes");
static_assert(sizeof(scg_rccr_t) == 4,     "Size must be 4 bytes");
static_assert(sizeof(scg_ldo_csr_t) == 4,  "Size must be 4 bytes");
static_assert(sizeof(scg_firc_cfg_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(scg_firc_csr_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(scg_sosc_csr_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(scg_sosc_cfg_t) == 4, "Size must be 4 bytes");

typedef enum { MHZ_48 = 0, MHZ_144 = 1 } ClkFreq;
typedef enum { SOSC_16_20MHZ = 0, SOSC_20_30MHZ = 1, SOSC_30_50MHZ = 2, SOSC_50_66MHZ = 3 } SOSCFreq;

const unsigned int scg_addr = 0x40044000;
const unsigned int scg_trim_lock_offset = 0x08;
const unsigned int scg_rccr_offset = 0x14;
const unsigned int scg_csr_offset = 0x10;
const unsigned int scg_sosc_csr_offset = 0x100;
const unsigned int scg_sosc_cfg_offset = 0x108;
const unsigned int scg_firc_csr_offset = 0x300;
const unsigned int scg_firc_cfg_offset = 0x308;
const unsigned int scg_ldo_csr_offset = 0x800;

// TODO: should only perform 32-bit writes on SCG registers otherwise we'll get a transfer error
ClkFreq get_firc_clk_freq(void) {
	volatile uint32_t* firc_cfg = (uint32_t*) (scg_addr + scg_firc_cfg_offset);
	return (*firc_cfg & 0x01);
}

int update_clk_freq(ClkFreq clk_freq) {
	volatile scg_firc_cfg_t* firc_cfg = (scg_firc_cfg_t*) (scg_addr + scg_firc_cfg_offset);
	volatile scg_firc_csr_t* firc_csr = (scg_firc_csr_t*) (scg_addr + scg_firc_csr_offset);
	volatile scg_rccr_t* rccr = (scg_rccr_t*) (scg_addr + scg_rccr_offset);
	volatile scg_csr_t*   csr = (scg_csr_t*) (scg_addr + scg_csr_offset);

	scg_firc_cfg_t firc_cfg_ = *firc_cfg;
	firc_cfg_.range = clk_freq & 0x01;
	*firc_cfg = firc_cfg_;

	scg_firc_csr_t firc_csr_ = *firc_csr;
	firc_csr_.lk = 0;
	firc_csr_.firc_fclk_periph_en = clk_freq & 0x01;
	firc_csr_.firc_sclk_periph_en = (~clk_freq) & 0x01;
	firc_csr_.firc_sten = 0;
	firc_csr_.firc_en = 1;
	*firc_csr = firc_csr_;

	while (!firc_csr -> firc_vld) delay_ms(5);
	if (firc_csr -> firc_err) return -1;

	scg_rccr_t rccr_ = *rccr;
	rccr_.scs = 3;
	*rccr = rccr_;
	while (csr -> scs != 3) delay_ms(5);

	return 0;
}

// SOSC and clk_in are the same clock (and their input reference is the external/internal clock oscillator)
int update_sosc_clk_freq(SOSCFreq sosc_freq) {
	volatile scg_sosc_csr_t* sosc_csr = (scg_sosc_csr_t*) (scg_addr + scg_sosc_csr_offset);
	volatile scg_sosc_cfg_t* sosc_cfg = (scg_sosc_cfg_t*) (scg_addr + scg_sosc_cfg_offset);
	volatile scg_ldo_csr_t* ldo_csr = (scg_ldo_csr_t*) (scg_addr + scg_ldo_csr_offset);
	volatile uint32_t* trim_lock = (uint32_t*) (scg_addr + scg_trim_lock_offset);

	set_enable_clk_ctrl(0, (1U << 13));
	scg_sosc_csr_t sosc_csr_ = *sosc_csr;
	sosc_csr_.lock = 0;
	sosc_csr_.enable = 0;
	*sosc_csr = sosc_csr_;
	*trim_lock = 0x5A5A0001; // Disable TRIM lock mechanism

	scg_ldo_csr_t ldo_csr_ = *ldo_csr;
	ldo_csr_.ldo_en = 1;
	*ldo_csr = ldo_csr_;
	while (!ldo_csr -> vout_ok) delay_us(1);

	scg_sosc_cfg_t sosc_cfg_ = *sosc_cfg;
	sosc_cfg_.erefs = 1;
	sosc_cfg_.range = sosc_freq & 0x03;
	*sosc_cfg = sosc_cfg_;

	sosc_csr_.sosc_cm = 1;
	sosc_csr_.sosc_sten = 1;
	sosc_csr_.enable = 1;
	*sosc_csr = sosc_csr_;
	while (!sosc_csr -> sosc_vld) delay_us(1);
	if (sosc_csr -> sosc_err) return -1;

	sosc_csr_.lock = 1;
	*sosc_csr = sosc_csr_;
	*trim_lock = 0x5A5A0000; // Reenable TRIM lock mechanism

	return 0;
}

#endif //_CLOCK_H_
