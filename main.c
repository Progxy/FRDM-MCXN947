#include <utils.h>
#include <debug_uart.h>
//#include <usb.h>
//#include "./ltr.h"
#include "./i2c_temp_sensor.h"

void main(void) {
	//init_usb_device(0, NULL, NULL);
	//init_i3c();
	//init_temp_sensor();
	//init_ltr();

	int err = init_debug_uart(115200);
	if (err) __asm__ ("bkpt");

	char buf[128] = {0};

	//uint32_t uvi = 0;
	unsigned int val = 7;
	while (TRUE) {
		//uvi = ltr_sample_uvi();
		//if (uvi > 3) __asm__("bkpt");
		memset(buf, 0, 128);
		const unsigned int buf_size = snprintf(buf, 128, "Hi mom %u\n", val) - 1;
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(3000);
		val++;
	}

	return;
}
