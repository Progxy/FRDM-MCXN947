#include <utils.h>
#include <debug_uart.h>
#include <els.h>

void main(void) {
	int err = 0;
	char buf[256] = {0};

	do {
		memset(buf, 0, 256);
		uint32_t r = rand(&err);
		if (err) __bkpt();
		const unsigned int buf_size = snprintf(buf, 256, "rand val: %lX.\n", r);
		err = send_uart(buf, buf_size);
		if (err) __bkpt();
		delay_ms(5000);
	} while (TRUE);

	return;
}
