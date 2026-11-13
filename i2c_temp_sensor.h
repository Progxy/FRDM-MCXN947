#ifndef _I2C_TEMP_SENSOR_H_
#define _I2C_TEMP_SENSOR_H_

#include <i3c.h>
#include <fpu.h>

/// Board temperature sensor NXP P3T1755
#define I2C_TEMP_SENSOR_ADDRESS 0x48

typedef enum { TEMP_REG = 0, CONFIG_REG = 1, T_LOW_REG = 2, T_HIGH_REG = 3 } PointerRegister;

int get_temp(void) {
	uint8_t pointer_reg_val = TEMP_REG;
	int status = __send_i2c(I3C1, I2C_TEMP_SENSOR_ADDRESS, &pointer_reg_val, 1, FALSE);
	if (status) __asm__("bkpt");

	uint8_t temp[2] = {0};
	status = __recv_i2c(I3C1, I2C_TEMP_SENSOR_ADDRESS, &temp, 2, TRUE);
	if (status) __asm__("bkpt");

	int t = ((temp[0] & 0x7F) << 4) | ((temp[1] >> 4) & 0x0F);
	if (temp[0] & 0x80) t = (~t + 1);

	return (int) ((float) t * 0.0625f);
}

uint8_t init_temp_sensor(void) {
	uint8_t pointer_reg_val = CONFIG_REG;
	int status = __send_i2c(I3C1, I2C_TEMP_SENSOR_ADDRESS, &pointer_reg_val, 1, FALSE);
	if (status) __asm__("bkpt");

	uint8_t config_reg = 0;
	status = __recv_i2c(I3C1, I2C_TEMP_SENSOR_ADDRESS, &config_reg, 1, TRUE);
	if (status) __asm__("bkpt");

	return config_reg;
}



#endif // _I2C_TEMP_SENSOR_H_
