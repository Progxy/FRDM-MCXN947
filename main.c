#include <stddef.h>
#include <utils.h>

void main(void) {
	volatile unsigned int cnt = 0;
	while (TRUE) {
		cnt++;
	}

	(void)cnt;

	return;
}
