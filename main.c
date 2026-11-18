#include <utils.h>
#include <debug_uart.h>
#include <usb.h>
#include <p3t1755.h>

void main(void) {
	int err = 0;
	char buf[256] = {0};

	err = init_default_usb_device();
	if (err) __asm__("bkpt");

	unsigned int buf_size = snprintf(buf, 256, "usb initialized.\n");
	err = send_uart(buf, buf_size);
	if (err) __asm__ ("bkpt");

	unsigned int val = 0;
	do {
		memset(buf, 0, 256);
		buf_size = snprintf(buf, 256, "val: %u\n", val++);
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(5000);
	} while (TRUE);

	return;
}
