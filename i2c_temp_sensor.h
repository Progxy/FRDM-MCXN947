#ifndef _I2C_TEMP_SENSOR_H_
#define _I2C_TEMP_SENSOR_H_

#include <i3c.h>

#define I2C_TEMP_SENSOR_ADDRESS 0x48

uint32_t get_temp(void) {
	return 0;
}

void init_temp_sensor(void) {
	return;
}

#endif // _I2C_TEMP_SENSOR_H_
