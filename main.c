#include <utils.h>
#include <debug_uart.h>
#include <enet.h>
//#include <usb.h>
//#include "./ltr.h"
//#include "./i2c_temp_sensor.h"

void main(void) {
	int err = 0;
	char buf[256] = {0};
	//init_ltr();
	// unsigned int config = init_temp_sensor();

	//test_enet_tx();

	uint32_t enet_status[15] = {0};
	do {
		get_enet_status(enet_status);
		memset(buf, 0, 256);
		unsigned int buf_size = snprintf(buf, 256, "enet status: ");
		for (unsigned int i = 0; i < 6 && buf_size < 256; ++i) buf_size += snprintf(buf + buf_size, 256 - buf_size, "%lX, ", enet_status[i]);
		buf_size += snprintf(buf + buf_size, 256 - buf_size, "\n");
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(3000);
	} while (TRUE);

	return;
}
