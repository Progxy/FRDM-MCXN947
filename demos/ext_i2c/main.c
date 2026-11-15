#include <utils.h>
#include <debug_uart.h>

void main(void) {
	int err = 0;
	char buf[256] = {0};

	do {
		memset(buf, 0, 256);
		const unsigned int _buf_size = snprintf(buf, 256, "...\n");
		err = send_uart(buf, _buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(5000);
	} while (TRUE);

	return;
}
