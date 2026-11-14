#include <utils.h>
#include <debug_uart.h>
#include "./ltr.h"
#include <p3t1755.h>
#include "scd.h"

void main(void) {
	int err = 0;
	char buf[256] = {0};
	//unsigned int config = p3t1755_get_config();
	//unsigned int buf_size = snprintf(buf, 256, "temp: %u\n", p3t1755_get_temp());
	//err = send_uart(buf, buf_size);
	//if (err) __asm__ ("bkpt");
	scd_init();

	scd_data_t scd_data = scd_sample();
	memset(buf, 0, 256);
	unsigned int buf_size = snprintf(buf, 256, "SCD %u, %u, %u\n", scd_data.c02, scd_data.t, scd_data.rh);
	err = send_uart(buf, buf_size);
	if (err) __asm__ ("bkpt");

	init_ltr();

	memset(buf, 0, 256);
	buf_size = snprintf(buf, 256, "LTR initialized\n");
	err = send_uart(buf, buf_size);
	if (err) __asm__ ("bkpt");

	do {
		memset(buf, 0, 256);
		buf_size = snprintf(buf, 256, "current temperature: %lu\n", ltr_sample_uvi());
		err = send_uart(buf, buf_size);
		if (err) __asm__ ("bkpt");
		delay_ms(5000);
	} while (TRUE);

	return;
}
