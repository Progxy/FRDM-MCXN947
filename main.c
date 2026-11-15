#include <utils.h>
#include <debug_uart.h>
#include <p3t1755.h>

void main(void) {
	int err = 0;
	char buf[256] = {0};
	unsigned int config = p3t1755_get_config();

	do {
		memset(buf, 0, 256);
		const unsigned int _buf_size = snprintf(buf, 256, "temp: %u\n", p3t1755_get_temp());
		err = send_uart(buf, _buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(5000);
	} while (TRUE);

	return;
}
