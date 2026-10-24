#include <stddef.h>
#include <utils.h>
#include <usb.h>
#include <i3c.h>

void main(void) {
	init_usb_device(0, NULL, NULL);
	init_i3c();

	volatile unsigned int cnt = 0;
	while (TRUE) {
		cnt++;
	}

	(void)cnt;

	return;
}
