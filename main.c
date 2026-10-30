#include <stddef.h>
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

	int err = init_debug_uart(0);
	if (err) __asm__ ("bkpt");

	const char buf[] = "Hello World!\r\n";
	const unsigned int buf_size = sizeof(buf);

	err = send_uart(buf, buf_size);
	if (err) __asm__ ("bkpt");

	uint32_t uvi = 0;
	while (TRUE) {
		//uvi = ltr_sample_uvi();
		//if (uvi > 3) __asm__("bkpt");
		delay_ms(3000);
	}

	return;
}
