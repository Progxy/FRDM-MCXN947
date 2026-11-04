#include <utils.h>
#include <debug_uart.h>
#include <enet.h>
//#include "./ltr.h"
#include "./i2c_temp_sensor.h"

void main(void) {
	int err = 0;
	char buf[128] = {0};
	//init_ltr();
	unsigned int config = init_temp_sensor();
	const unsigned int buf_size = snprintf(buf, 128, "config reg: %X\n", config) - 1;
	err = send_uart(buf, buf_size);
	if (err) __asm__ ("bkpt");

	unsigned int val = 7;
	while (TRUE) {
		memset(buf, 0, 128);
		const unsigned int buf_size = snprintf(buf, 128, "Hi mom %u\n", val) - 1;
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(3000);
		val++;
	}

	return;
}
