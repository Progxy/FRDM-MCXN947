#include <stddef.h>
#include <utils.h>
#include <usb.h>

void main(void) {
	init_usb_device(0, NULL, NULL);

	volatile unsigned int cnt = 0;
	while (TRUE) {
		cnt++;
	}

	(void)cnt;

	return;
}
