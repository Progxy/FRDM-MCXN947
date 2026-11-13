#include <utils.h>
#include <debug_uart.h>
#include "./ltr.h"

void main(void) {
	int err = 0;
	char buf[256] = {0};
	init_ltr();
	const unsigned int buf_size = snprintf(buf, 256, "LTR initialized\n");
	err = send_uart(buf, buf_size);
	if (err) __asm__ ("bkpt");

	do {
		memset(buf, 0, 256);
		const unsigned int buf_size = snprintf(buf, 256, "current temperature: %lu\n", ltr_sample_uvi());
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(5000);
	} while (TRUE);

	return;
}
