#include <utils.h>
#include <debug_uart.h>
// #include <enet.h>
//#include <usb.h>
//#include "./ltr.h"
#include "./i2c_temp_sensor.h"

void main(void) {
	int err = 0;
	char buf[256] = {0};
	//init_ltr();
	unsigned int config = init_temp_sensor();
	const unsigned int buf_size = snprintf(buf, 256, "config: %X\n", config);
	err = send_uart(buf, buf_size);
	if (err) __asm__ ("bkpt");

	unsigned int val = 0;
	do {
		memset(buf, 0, 256);
		const unsigned int buf_size = snprintf(buf, 256, "val: %u\n", val++);
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(3000);
	} while (TRUE);

	return;
}
