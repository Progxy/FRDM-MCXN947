#include <utils.h>
#include <debug_uart.h>
#include <enet.h>
//#include <usb.h>
//#include "./ltr.h"
//#include "./i2c_temp_sensor.h"

void main(void) {
	int err = 0;
	char buf[128] = {0};
	//init_ltr();
	// unsigned int config = init_temp_sensor();

	test_enet_tx();

	uint32_t enet_status[4] = {0};
	do {
		get_enet_status(enet_status);
		for (unsigned int i = 0; i < 4; ++i) if (enet_status[i]) __asm__("bkpt");
		memset(buf, 0, 128);
		const unsigned int buf_size = snprintf(buf, 128, "enet status: %lX, %lX, %lX, %lX\n", enet_status[0], enet_status[1], enet_status[2], enet_status[3]) - 1;
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(3000);
	} while (TRUE);

	return;
}
