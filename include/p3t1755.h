#ifndef _P3T1755_H_
#define _P3T1755_H_

#include <i3c.h>
#include <fpu.h>

/// On-board temperature sensor NXP P3T1755
#define P3T1755_ADDRESS 0x48

typedef struct {
	uint8_t sd: 1;
	uint8_t tm: 1;
	uint8_t pol: 1;
	uint8_t f0: 1;
	uint8_t f1: 1;
	uint8_t r0: 1;
	uint8_t r1: 1;
	uint8_t os: 1;
} __attribute__((packed)) p3t1755_cfg_reg_t;

static_assert(sizeof(p3t1755_cfg_reg_t) == 1, "Size must be one byte.");

typedef enum { TEMP_REG = 0, CONFIG_REG = 1, T_LOW_REG = 2, T_HIGH_REG = 3 } PointerRegister;

int p3t1755_get_temp(void) {
	uint8_t pointer_reg_val = TEMP_REG;
	int status = __send_i2c(I3C1, P3T1755_ADDRESS, &pointer_reg_val, 1, FALSE);
	if (status) __asm__("bkpt");

	uint8_t temp[2] = {0};
	status = __recv_i2c(I3C1, P3T1755_ADDRESS, &temp, 2, TRUE);
	if (status) __asm__("bkpt");

	int t = ((temp[0] & 0x7F) << 4) | ((temp[1] >> 4) & 0x0F);
	if (temp[0] & 0x80) t = (~t + 1);

	return (int) ((float) t * 0.0625f);
}

uint8_t p3t1755_get_config(void) {
	uint8_t pointer_reg_val = CONFIG_REG;
	int status = __send_i2c(I3C1, P3T1755_ADDRESS, &pointer_reg_val, 1, FALSE);
	if (status) __asm__("bkpt");

	uint8_t config_reg = 0;
	status = __recv_i2c(I3C1, P3T1755_ADDRESS, &config_reg, 1, TRUE);
	if (status) __asm__("bkpt");

	return config_reg;
}

void p3t1755_set_config(p3t1755_cfg_reg_t config_reg) {
	uint8_t data[2] = { CONFIG_REG, 0 };
	data[1] = *((uint8_t*) &config_reg);
	int status = __send_i2c(I3C1, P3T1755_ADDRESS, &data, 2, TRUE);
	if (status) __asm__("bkpt");
	return;
}

#endif // _P3T1755_H_
