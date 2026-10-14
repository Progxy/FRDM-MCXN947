#ifndef _POWER_H_
#define _POWER_H_

#include <stdint.h>
#include "./utils.h"
#include "./clock.h"
#include "./fmu.h"

typedef struct {
	uint8_t coreldo_vd_ds: 1;
	uint8_t rsv: 1;
	uint8_t coreldo_vdd_lvl: 2;
	uint8_t sysldo_vdd_ds: 1;
	uint8_t rsv1: 1;
	uint8_t sysldo_vdd_lvl: 1;
	uint8_t rsv2: 1;

	uint8_t dcdc_vdd_ds: 2;
	uint8_t dcdc_vdd_lvl: 2;
	uint8_t glitch_detect_disable: 1;
	uint8_t rsv3a: 3;

	uint8_t rsv3b: 2;
	uint8_t lpbuff_en: 1;
	uint8_t rsv4: 1;
	uint8_t bgmode: 2;
	uint8_t rsv5: 1;
	uint8_t vdd_vd_disable: 1;

	uint8_t core_lvde: 1;
	uint8_t sys_lvde: 1;
	uint8_t io_lvde: 1;
	uint8_t core_hvde: 1;
	uint8_t sys_hvde: 1;
	uint8_t io_hvde: 1;
	uint8_t rsv6: 2;
} __attribute__((packed)) spc_active_cfg_t;

typedef struct {
	uint8_t busy: 1;
	uint8_t spc_lp_req: 1;
	uint8_t rsv: 2;
	uint8_t spc_lp_mode: 4;
	uint8_t rsv2;
	uint8_t iso_clr: 2;
	uint16_t rsv3: 14;
} __attribute__((packed)) spc_sc_t;

typedef struct {
	uint8_t vsm: 2;
	uint32_t rsv: 28;
	uint8_t req: 1;
	uint8_t ack: 1;
} __attribute__((packed)) spc_sram_ctl_t;

static_assert(sizeof(spc_sc_t) == 4,         "Size must be 4 bytes");
static_assert(sizeof(spc_sram_ctl_t) == 4,   "Size must be 4 bytes");
static_assert(sizeof(spc_active_cfg_t) == 4, "Size must be 4 bytes");

const unsigned int spc_addr = 0x40045000;
const unsigned int spc_sc_offset = 0x10;
const unsigned int spc_active_cfg_offset = 0x100;
const unsigned int spc_sram_ctl_offset = 0x40;
const unsigned int spc_active_vdelay_offset = 0x124;

// NOTE: should only perform 32-bit writes on SPC registers otherwise we'll get a transfer error
int increase_voltage_level(volatile spc_active_cfg_t* spc_active_cfg_addr, unsigned int voltage_lvl) {
	volatile uint32_t* spc_active_vdelay = (uint32_t*) (spc_addr + spc_active_vdelay_offset);
	*spc_active_vdelay = 0x500;

	spc_active_cfg_t spc_active_cfg = *spc_active_cfg_addr;
	spc_active_cfg.dcdc_vdd_lvl = voltage_lvl & 0x03;
	spc_active_cfg.coreldo_vdd_lvl = voltage_lvl & 0x03;
	*spc_active_cfg_addr = spc_active_cfg;

	volatile spc_sc_t* sc = (spc_sc_t*) (spc_addr + spc_sc_offset);
	while (sc -> busy) delay_us(5);

	set_fctrl_rwsc(3);

	volatile spc_sram_ctl_t* sram_ctl = (spc_sram_ctl_t*) (spc_addr + spc_sram_ctl_offset);
	spc_sram_ctl_t sram_ctl_ = *sram_ctl;
	sram_ctl_.vsm = 2;
	sram_ctl_.req = 1;
	*sram_ctl = sram_ctl_;
	while (!sram_ctl -> ack) delay_us(5);

	sram_ctl_.req = 0;
	*sram_ctl = sram_ctl_;

	//int err = update_clk_freq(MHZ_144);
	//return err;
	return 0;
}

int decrease_voltage_level(volatile spc_active_cfg_t* spc_active_cfg_addr, unsigned int voltage_lvl) {
	int err = update_clk_freq(MHZ_48);
	if (err) return err;

	set_fctrl_rwsc(1);

	volatile spc_sram_ctl_t* sram_ctl = (spc_sram_ctl_t*) (spc_addr + spc_sram_ctl_offset);
	spc_sram_ctl_t sram_ctl_ = *sram_ctl;
	sram_ctl_.vsm = 1;
	sram_ctl_.req = 1;
	*sram_ctl = sram_ctl_;
	while (!sram_ctl -> ack) delay_us(5);

	sram_ctl_.req = 1;
	*sram_ctl = sram_ctl_;

	spc_active_cfg_t spc_active_cfg = *spc_active_cfg_addr;
	spc_active_cfg.dcdc_vdd_lvl    = voltage_lvl & 0x03;
	spc_active_cfg.coreldo_vdd_lvl = voltage_lvl & 0x03;
	*spc_active_cfg_addr = spc_active_cfg;

	volatile spc_sc_t* sc = (spc_sc_t*) (spc_addr + spc_sc_offset);
	while (sc -> busy) delay_us(5);

	return 0;
}

int change_dcdc_voltage_lvl(unsigned int voltage_lvl) {
	if (voltage_lvl < 1 || voltage_lvl > 3) return -1;

	int err = 0;
	volatile spc_active_cfg_t* spc_active_cfg_addr = (spc_active_cfg_t*) (spc_addr + spc_active_cfg_offset);
	const spc_active_cfg_t active_cfg = *spc_active_cfg_addr;
	if (active_cfg.dcdc_vdd_lvl > voltage_lvl) {
		err = decrease_voltage_level(spc_active_cfg_addr, voltage_lvl);
	} else if (active_cfg.dcdc_vdd_lvl < voltage_lvl) {
		err = increase_voltage_level(spc_active_cfg_addr, voltage_lvl);
	}

	return err;
}

#endif //_POWER_H_
