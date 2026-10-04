#ifndef _FMU_H_
#define _FMU_H_

typedef struct {
	uint8_t rwsc: 4;
	const uint16_t rsv: 12;
	uint8_t fdfd: 1;
	const uint8_t rsv2: 7;
	uint8_t abtreq: 1;
	const uint8_t rsv3: 7;
} __attribute__((packed)) fctrl_t;

static_assert(sizeof(fctrl_t) == 4, "Size must be 4 bytes");

const unsigned int fmu0_addr = 0x40043000;
const unsigned int fctrl_offset = 0x08;

void set_fctrl_rwsc(unsigned int rswc) {
	volatile fctrl_t* fctrl = (fctrl_t*) (fmu0_addr + fctrl_offset);
	fctrl -> rwsc = rswc & 0x0F;
	return;
}

#endif //_FMU_H_
