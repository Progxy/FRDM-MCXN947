#ifndef _POWER_H_
#define _POWER_H_

#include <stdint.h>
#include "./utils.h"
#include "./clock.h"
#include "./fmu.h"

typedef struct {
	uint8_t coreldo_vd_ds: 1;
	const uint8_t rsv: 1;
	uint8_t coreldo_vdd_lvl: 2;
	uint8_t sysldo_vdd_ds: 1;
	const uint8_t rsv1: 1;
	uint8_t sysldo_vdd_lvl: 1;
	const uint8_t rsv2: 1;

	uint8_t dcdc_vdd_ds: 2;
	uint8_t dcdc_vdd_lvl: 2;
	uint8_t glitch_detect_disable: 1;
	const uint8_t rsv3a: 3;

	const uint8_t rsv3b: 2;
	uint8_t lpbuff_en: 1;
	const uint8_t rsv4: 1;
	uint8_t bgmode: 2;
	const uint8_t rsv5: 1;
	uint8_t vdd_vd_disable: 1;

	uint8_t core_lvde: 1;
	uint8_t sys_lvde: 1;
	uint8_t io_lvde: 1;
	uint8_t core_hvde: 1;
	uint8_t sys_hvde: 1;
	uint8_t io_hvde: 1;
	const uint8_t rsv6: 2;
} __attribute__((packed)) spc_active_cfg_t;

typedef struct {
	uint8_t busy: 1;
	uint8_t spc_lp_req: 1;
	const uint8_t rsv: 2;
	uint8_t spc_lp_mode: 4;
	const uint8_t rsv2;
	uint8_t iso_clr: 2;
	const uint16_t rsv3: 14;
} __attribute__((packed)) spc_sc_t;

typedef struct {
	uint8_t vsm: 2;
	const uint32_t rsv: 28;
	uint8_t req: 1;
	const uint8_t ack: 1;
} __attribute__((packed)) spc_sram_ctl_t;

static_assert(sizeof(spc_sc_t) == 4,         "Size must be 4 bytes");
static_assert(sizeof(spc_sram_ctl_t) == 4,   "Size must be 4 bytes");
static_assert(sizeof(spc_active_cfg_t) == 4, "Size must be 4 bytes");

const unsigned int spc_addr = 0x40045000;
const unsigned int spc_sc_offset = 0x10;
const unsigned int spc_active_cfg_offset = 0x100;
const unsigned int spc_sram_ctl_offset = 0x40;

int increase_voltage_level(volatile spc_active_cfg_t* spc_active_cfg_addr, unsigned int voltage_lvl) {
	spc_active_cfg_addr -> dcdc_vdd_lvl = voltage_lvl & 0x03;
	spc_active_cfg_addr -> coreldo_vdd_lvl = voltage_lvl & 0x03;

	volatile spc_sc_t* sc = (spc_sc_t*) (spc_addr + spc_sc_offset);
	while (sc -> busy) delay_ms(5);

	set_fctrl_rwsc(3);

	const unsigned int sram_ctl_offset = 0x40;
	volatile spc_sram_ctl_t* sram_ctl = (spc_sram_ctl_t*) (spc_addr + spc_sram_ctl_offset);
	sram_ctl -> vsm = 2;
	sram_ctl -> req = 1;

	while (!sram_ctl -> ack) delay_ms(5);

	sram_ctl -> req = 0;
	int err = update_clk_freq(MHZ_144);

	return err;
}

int decrease_voltage_level(volatile spc_active_cfg_t* spc_active_cfg_addr, unsigned int voltage_lvl) {
	int err = update_clk_freq(MHZ_144);
	if (err) return err;

	set_fctrl_rwsc(1);

	volatile spc_sram_ctl_t* sram_ctl = (spc_sram_ctl_t*) (spc_addr + spc_sram_ctl_offset);
	sram_ctl -> vsm = 1;
	sram_ctl -> req = 1;

	while (!sram_ctl -> ack) delay_ms(5);

	sram_ctl -> req = 1;
	spc_active_cfg_addr -> dcdc_vdd_lvl = voltage_lvl & 0x03;
	spc_active_cfg_addr -> coreldo_vdd_lvl = voltage_lvl & 0x03;

	volatile spc_sc_t* sc = (spc_sc_t*) (spc_addr + spc_sc_offset);
	while (sc -> busy) delay_ms(5);

	return 0;
}

int change_dcdc_voltage_lvl(unsigned int voltage_lvl) {
	if (voltage_lvl < 1 || voltage_lvl > 3) return -1;

	int err = 0;
	volatile spc_active_cfg_t* spc_active_cfg_addr = (spc_active_cfg_t*) (spc_addr + spc_active_cfg_offset);
	const spc_active_cfg_t active_cfg = *spc_active_cfg_addr;
	if (active_cfg.dcdc_vdd_lvl > voltage_lvl) {
		err = decrease_voltage_level(spc_active_cfg_addr, voltage_lvl);
	} else if (active_cfg.dcdc_vdd_lvl > voltage_lvl) {
		err = increase_voltage_level(spc_active_cfg_addr, voltage_lvl);
	}

	return err;
}

#endif //_POWER_H_
