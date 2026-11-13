#include <utils.h>
#include <debug_uart.h>
#include <p3t1755.h>

void main(void) {
	int err = 0;
	char buf[256] = {0};
	unsigned int config = p3t1755_get_config();
	const unsigned int buf_size = snprintf(buf, 256, "config: %X\n", config);
	err = send_uart(buf, buf_size);
	if (err) __asm__ ("bkpt");

	do {
		memset(buf, 0, 256);
		const unsigned int buf_size = snprintf(buf, 256, "current temperature: %d\n", p3t1755_get_temp());
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(5000);
	} while (TRUE);

	return;
}
