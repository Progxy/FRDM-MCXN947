#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>
#include "./utils.h"

typedef struct {
	uint8_t range: 1;
	const uint32_t rsv: 31;
} __attribute__((packed)) scg_firc_cfg_t;

typedef struct {
	uint8_t firc_en: 1;
	uint8_t firc_sten: 1;
	const uint8_t rsv: 2;
	uint8_t firc_sclk_periph_en: 1;
	uint8_t firc_fclk_periph_en: 1;
	const uint8_t rsv1: 2;
	uint8_t firc_tren: 1;
	uint8_t firc_trup: 1;
	uint8_t trim_lock: 1;
	uint8_t coarse_trim_bypass: 1;
	const uint16_t rsv2: 11;
	uint8_t lk: 1;
	uint8_t firc_vld: 1;
	uint8_t firc_sel: 1;
	uint8_t firc_err: 1;
	uint8_t fircerr_i: 1;
	const uint8_t rsv3: 2;
	uint8_t firc_acc_ie: 1;
	uint8_t firc_acc: 1;
} __attribute__((packed)) scg_firc_csr_t;

typedef struct {
	const uint32_t rsv: 24;
	uint8_t scs: 4;
	const uint8_t rsv1: 4;
} __attribute__((packed)) scg_rccr_t;

typedef scg_rccr_t scg_csr_t;

static_assert(sizeof(scg_csr_t) == 4,      "Size must be 4 bytes");
static_assert(sizeof(scg_rccr_t) == 4,     "Size must be 4 bytes");
static_assert(sizeof(scg_firc_cfg_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(scg_firc_csr_t) == 4, "Size must be 4 bytes");

typedef enum { MHZ_48 = 0, MHZ_144 = 1 } ClkFreq;

const unsigned int scg_addr = 0x40044000;
const unsigned int scg_firc_cfg_offset = 0x308;
const unsigned int scg_firc_csr_offset = 0x300;
const unsigned int scg_rccr_offset = 0x14;
const unsigned int scg_csr_offset  = 0x10;

int update_clk_freq(ClkFreq clk_freq) {
	volatile scg_firc_cfg_t* firc_cfg = (scg_firc_cfg_t*) (scg_addr + scg_firc_cfg_offset);
	volatile scg_firc_csr_t* firc_csr = (scg_firc_csr_t*) (scg_addr + scg_firc_csr_offset);
	volatile scg_rccr_t* rccr = (scg_rccr_t*) (scg_addr + scg_rccr_offset);
	volatile scg_csr_t*   csr = (scg_csr_t*) (scg_addr + scg_csr_offset);

	firc_cfg -> range = clk_freq & 0x01;
	firc_csr -> lk = 0;
	firc_csr -> firc_fclk_periph_en = clk_freq & 0x01;
	firc_csr -> firc_sclk_periph_en = (~clk_freq) & 0x01;
	firc_csr -> firc_sten = 0;
	firc_csr -> firc_en = 1;

	while (!firc_csr -> firc_vld) delay_ms(5);
	if (firc_csr -> firc_err) return -1;

	rccr -> scs = 3;
	while (csr -> scs != 3) delay_ms(5);

	return 0;
}

#endif //_CLOCK_H_
