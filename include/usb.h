#ifndef _USB_H_
#define _USB_H_

#include <stdint.h>
#include "./utils.h"
#include "./alloc.h"
#include "./syscon.h"
#include "./power.h"

#define USB_MAX_HW_RESETS 5

typedef struct {
	union {
		struct {
			const uint16_t rsv2: 15;
			const uint8_t ios: 1;
			uint16_t max_packet_len: 11;
			const uint8_t rsv: 2;
			const uint8_t zlt: 1;
			const uint8_t mult: 2;

			uint32_t current_dtd_ptr;
			union {
				struct {
					uint8_t t: 1;
					uint32_t next_dtd_ptr: 31;
				};
				uint32_t next_ptr;
			};

			uint8_t status;
			const uint8_t rsv5: 2;
			const uint8_t mult_o: 2;
			const uint8_t rsv4: 3;
			uint8_t ioc: 1;
			uint16_t total_bytes: 15;
			const uint8_t rsv3: 1;

			uint16_t current_offset: 12;
			uint32_t buffer_ptr0: 20;

			const uint16_t rsv6: 12;
			uint32_t buffer_ptr1: 20;

			const uint16_t rsv7: 12;
			uint32_t buffer_ptr2: 20;

			const uint16_t rsv8: 12;
			uint32_t buffer_ptr3: 20;

			const uint16_t rsv9: 12;
			uint32_t buffer_ptr4: 20;

			const uint32_t rsv10;
			uint8_t setup_buffer[8];
		};

		uint8_t data[64];
	};
} __attribute__((packed)) endpoint_queue_head_t;

typedef struct {
	union {
		struct {
			uint8_t t: 1;
			const uint8_t rsv1: 4;
			uint32_t next_link_ptr: 27;
		};

		uint32_t next_ptr;
	};
	uint8_t status;

	const uint8_t rsv5: 2;
	const uint8_t mult_o: 2;
	const uint8_t rsv4: 3;
	uint8_t ioc: 1;

	uint16_t total_bytes: 15;
	const uint8_t rsv3: 1;

	uint16_t current_offset: 12;
	uint32_t buffer_ptr0: 20;

	const uint16_t frame_number: 11;
	const uint8_t rsv6: 1;
	uint32_t buffer_ptr1: 20;

	const uint16_t rsv7: 12;
	uint32_t buffer_ptr2: 20;

	const uint16_t rsv8: 12;
	uint32_t buffer_ptr3: 20;

	const uint16_t rsv9: 12;
	uint32_t buffer_ptr4: 20;

	// Extra data that should be ignored by the controller
	uint32_t prev_link_ptr;

} __attribute__((packed)) endpoint_transfer_descriptor_t;

typedef struct {
	uint8_t rs: 1;
	uint8_t rst: 1;
	uint8_t fs_1: 2;
	uint8_t pse: 1;
	uint8_t ase: 1;
	uint8_t iaa: 1;
	const uint8_t rsv: 1;
	uint8_t asp: 2;
	const uint8_t rsv1: 1;
	uint8_t aspe: 1;
	const uint8_t rsv2: 1;
	uint8_t sutw: 1;
	uint8_t atdtw: 1;
	uint8_t fs_2: 1;
	uint8_t itc;
	const uint8_t rsv3;
} __attribute__((packed)) usb_cmd_t;

typedef struct {
	uint8_t ui: 1;
	uint8_t uei: 1;
	uint8_t pci: 1;
	uint8_t fri: 1;
	uint8_t sei: 1;
	uint8_t aai: 1;
	uint8_t uri: 1;
	uint8_t sri: 1;
	uint8_t sli: 1;
	const uint8_t rsv: 1;
	uint8_t ulpii: 1;
	const uint8_t rsv1: 1;
	uint8_t hch: 1;
	uint8_t rcl: 1;
	uint8_t ps: 1;
	uint8_t as: 1;
	uint8_t naki: 1;
	const uint8_t rsv2: 7;
	uint8_t ti0: 1;
	uint8_t ti1: 1;
	const uint8_t rsv3: 6;
} __attribute__((packed)) usb_status_t;

typedef struct {
	uint8_t ue: 1;
	uint8_t uee: 1;
	uint8_t pce: 1;
	uint8_t fre: 1;
	uint8_t see: 1;
	uint8_t aae: 1;
	uint8_t ure: 1;
	uint8_t sre: 1;
	uint8_t sle: 1;
	const uint8_t rsv: 7;
	uint8_t nake: 1;
	const uint8_t rsv1: 1;
	uint8_t uaie: 1;
	uint8_t upie: 1;
	const uint8_t rsv2: 4;
	uint8_t tie0: 1;
	uint8_t tie1: 1;
	const uint8_t rsv3: 6;
} __attribute__((packed)) usb_int_enable_t;

typedef struct {
	const uint16_t rsv: 11;
	uint32_t epbase: 21;
} __attribute__((packed)) usb_endptlistaddr_t;

typedef struct {
	uint8_t rxs: 1;
	uint8_t rxd: 1;
	uint8_t rxt: 2;
	const uint8_t rsv1: 1;
	uint8_t rxi: 1;
	uint8_t rxr: 1;
	uint8_t rxe: 1;
	const uint8_t rsv2;
	uint8_t txs: 1;
	uint8_t txd: 1;
	uint8_t txt: 2;
	const uint8_t rsv3: 1;
	uint8_t txi: 1;
	uint8_t txr: 1;
	uint8_t txe: 1;
	const uint8_t rsv4;
} __attribute__((packed)) usb_endptctl_t;

typedef struct {
	const uint8_t ccs: 1;
	uint8_t csc: 1;
	uint8_t pe: 1;
	uint8_t pec: 1;
	const uint8_t oca: 1;
	uint8_t occ: 1;
	uint8_t fpr: 1;
	uint8_t susp: 1;

	uint8_t pr: 1;
	const uint8_t hsp: 1;
	const uint8_t ls: 2;
	uint8_t pp: 1;
	const uint8_t po: 1;
	uint8_t pic: 2;

	uint8_t ptc: 4;
	uint8_t wkcn: 1;
	uint8_t wkdc: 1;
	uint8_t wkoc: 1;
	uint8_t phcd: 1;

	uint8_t pfsc: 1;
	const uint8_t pts2: 1;
	uint8_t pspd: 2;
	const uint8_t ptw: 1;
	const uint8_t sts: 1;
	const uint8_t pts1: 2;
} __attribute__((packed)) usb_portsc1_t;

typedef struct {
	uint8_t erbr;
	const uint8_t rsv;
	uint8_t etbr;
	const uint8_t rsv1;
} __attribute__((packed)) endpt_stat_t;

typedef struct {
	uint8_t perb;
	const uint8_t rsv;
	uint8_t petb;
	const uint8_t rsv1;
} __attribute__((packed)) endpt_prime_t;

typedef struct {
	uint8_t cm: 2;
	uint8_t es: 1;
	uint8_t slom: 1;
	uint8_t sdis: 1;
	const uint32_t rsv: 27;
} __attribute__((packed)) usb_mode_t;

typedef enum { IDLE = 0, DEVICE = 2, HOST = 3 } USBMode;
typedef enum { CONTROL = 0, ISOCHRONOUS = 1, BULK = 2, INTERRUPT = 3 } USBType;
typedef enum { TRANSACTION_ERROR = 3, DATA_BUFFER_ERROR = 5, HALTED = 6, ACTIVE = 7 } USBStatus;

static_assert(sizeof(usb_cmd_t) == 4,                       "Size must be 4 bytes");
static_assert(sizeof(usb_mode_t) == 4,                      "Size must be 4 bytes");
static_assert(sizeof(usb_status_t) == 4,                    "Size must be 4 bytes");
static_assert(sizeof(endpt_stat_t) == 4,                    "Size must be 4 bytes");
static_assert(sizeof(endpt_prime_t) == 4,                   "Size must be 4 bytes");
static_assert(sizeof(usb_portsc1_t) == 4,                   "Size must be 4 bytes");
static_assert(sizeof(usb_endptctl_t) == 4,                  "Size must be 4 bytes");
static_assert(sizeof(usb_int_enable_t) == 4,                "Size must be 4 bytes");
static_assert(sizeof(usb_endptlistaddr_t) == 4,             "Size must be 4 bytes");
static_assert(sizeof(endpoint_queue_head_t) == 64,          "Size must be 64 bytes");
static_assert(sizeof(endpoint_transfer_descriptor_t) == 32, "Size must be 32 bytes");

const unsigned int usb_addr =  0x4010B000;
const unsigned int usb_cmd_offset = 0x140;
const unsigned int usb_status_offset = 0x144;
const unsigned int usb_int_enable_offset = 0x148;
const unsigned int usb_endptlistaddr_offset = 0x158;
const unsigned int usb_mode_offset = 0x1A8;
const unsigned int usb_endptctl_offset = 0x1C0;
const unsigned int endpt_stat_offset = 0x1B8;
const unsigned int endpt_complete_offset = 0x1BC;
const unsigned int endpt_prime_offset = 0x1B0;
const unsigned int endpt_flust_offset = 0x1B4;

endpoint_transfer_descriptor_t* epts_tdt_heads[32] = {0};
endpoint_queue_head_t* device_queue_heads = NULL;

int init_transfer_descriptor(endpoint_transfer_descriptor_t* tdt, const uint8_t* buf, const uint16_t total_bytes) {
	if (buf == NULL) return -1;
	tdt -> t = 1;
	tdt -> total_bytes = total_bytes;

	// OPTIONAL: This bit is used to indicate if USBINT is to be set in response
	//           to device controller being finished with this dTD.
	tdt -> ioc = 1;
	tdt -> status = 1U << ACTIVE;
	tdt -> buffer_ptr0    = mask_lower_bits((uint32_t) buf, 12);
	tdt -> current_offset = mask_upper_bits((uint32_t) buf, 12);
	tdt -> buffer_ptr1 = tdt -> buffer_ptr0 + 1;
	tdt -> buffer_ptr2 = tdt -> buffer_ptr1 + 1;
	tdt -> buffer_ptr3 = tdt -> buffer_ptr2 + 1;
	tdt -> buffer_ptr4 = tdt -> buffer_ptr3 + 1;

	return 0;
}

void init_device_queue_head(endpoint_queue_head_t* dqh) {
	dqh -> max_packet_len = 512;
	dqh -> t = 1;
	return;
}

void add_dtd_to_dqh(endpoint_transfer_descriptor_t* tdt, endpoint_queue_head_t* dqh) {
	const unsigned int ept_idx = ((unsigned int) dqh >> 6) & 0x1F;
	volatile endpt_prime_t* endpt_prime = (endpt_prime_t*) (usb_addr + endpt_prime_offset);
	volatile endpt_stat_t* endpt_stat = (endpt_stat_t*) (usb_addr + endpt_stat_offset);

	if (epts_tdt_heads[ept_idx] == NULL) {
		dqh -> next_ptr = mask_lower_bits((uint32_t) tdt, 5);
		epts_tdt_heads[ept_idx] = tdt;
		dqh -> status &= ~((1U << ACTIVE) | (1U << HALTED));
		if (ept_idx & 0x1) endpt_prime -> perb |= 1U << (ept_idx >> 1);
		else endpt_prime -> petb = 1U << (ept_idx >> 1);

		if (ept_idx & 0x1) {
			while (!((endpt_stat -> erbr >> (ept_idx >> 1)) & 0x01)) delay_ms(5);
		} else {
			while (!((endpt_stat -> etbr >> (ept_idx >> 1)) & 0x01)) delay_ms(5);
		}

		return;
	}

	uint8_t status = 0;
	do {
		endpoint_transfer_descriptor_t* current_tdt = epts_tdt_heads[ept_idx];
		while (current_tdt -> next_link_ptr != 0) current_tdt = (endpoint_transfer_descriptor_t*) current_tdt -> next_ptr;
		tdt -> prev_link_ptr = (unsigned int) current_tdt;
		current_tdt -> next_ptr = mask_lower_bits((uint32_t) tdt, 5);

		uint8_t is_primed = 0;
		if (ept_idx & 0x1) is_primed = endpt_prime -> perb |= 1U << (ept_idx >> 1);
		else is_primed = endpt_prime -> petb = 1U << (ept_idx >> 1);

		if (is_primed) return;

		volatile usb_cmd_t* usb_cmd = (usb_cmd_t*) (usb_addr + usb_cmd_offset);
		do {
			usb_cmd -> atdtw = 1;
			if (ept_idx & 0x1) status = (endpt_stat -> erbr >> (ept_idx >> 1)) & 0x01;
			else status = (endpt_stat -> etbr >> (ept_idx >> 1)) & 0x01;
		} while (!usb_cmd -> atdtw);

		usb_cmd -> atdtw = 0;
	} while (status == 0);

	return;
}

void init_endpoint(unsigned int endpt_idx, USBType usb_type) {
	if (endpt_idx > 7) return;
	volatile usb_endptctl_t* endptctl = (usb_endptctl_t*) (usb_endptctl_offset + 4 * endpt_idx);

	endptctl -> rxs = 0;
	endptctl -> rxd = 0;
	endptctl -> rxt = usb_type;
	endptctl -> rxi = 0;
	endptctl -> rxr = 1;
	endptctl -> rxe = 1;

	endptctl -> txs = 0;
	endptctl -> txd = 0;
	endptctl -> txt = usb_type;
	endptctl -> txi = 0;
	endptctl -> txr = 1;
	endptctl -> txe = 1;

	return;
}

int reset_usb_controller(const uint8_t endpts_cnt) {
	volatile usb_cmd_t* usb_cmd = (usb_cmd_t*) (usb_addr + usb_cmd_offset);
	usb_cmd -> rs = 0;

	volatile usb_status_t* usb_status = (usb_status_t*) (usb_addr + usb_status_offset);
	while (!usb_status -> hch) delay_ms(5);

	usb_cmd -> rst = 1;
	while (usb_cmd -> rst) delay_ms(5);

	volatile usb_mode_t* usb_mode = (usb_mode_t*) (usb_addr + usb_mode_offset);
	usb_mode -> cm = DEVICE;

	device_queue_heads = calloc(sizeof(endpoint_queue_head_t), endpts_cnt);
	if (device_queue_heads == NULL) return -1;

	for (unsigned int i = 0; i < endpts_cnt; ++i) init_device_queue_head(device_queue_heads + i);

	volatile usb_endptlistaddr_t* usb_endptlistaddr = (usb_endptlistaddr_t*) (usb_addr + usb_endptlistaddr_offset);
	usb_endptlistaddr -> epbase = mask_lower_bits((uint32_t) device_queue_heads, 11);

	volatile usb_int_enable_t* usb_int_enable = (usb_int_enable_t*) (usb_addr + usb_int_enable_offset);
	usb_int_enable -> ue = 1;
	usb_int_enable -> uee = 1;
	usb_int_enable -> pce = 1;
	usb_int_enable -> ure = 1;
	usb_int_enable -> sle = 1;

	usb_cmd -> rs = 0;
	while (usb_status -> hch) delay_ms(5);

	return 0;
}

int handle_bus_reset(void) {
	volatile uint32_t* endpt_stat = (uint32_t*) (usb_addr + endpt_stat_offset);
	*endpt_stat = *endpt_stat;

	volatile uint32_t* endpt_complete = (uint32_t*) (usb_addr + endpt_complete_offset);
	*endpt_complete = *endpt_complete;

	volatile uint32_t* endpt_prime = (uint32_t*) (usb_addr + endpt_prime_offset);
	while (*endpt_prime) delay_ms(5);

	volatile uint32_t* endpt_flush = (uint32_t*) (usb_addr + endpt_flust_offset);
	*endpt_flush = 0xFFFFFFFF;

	const unsigned int usb_portsc1_offset = 0x184;
	volatile usb_portsc1_t* usb_portsc1 = (usb_portsc1_t*) (usb_addr + usb_portsc1_offset);
	if (usb_portsc1 -> pr == 0) return 1;

	return 0;
}

// TODO: Theoretically should ensure that the MCU is running in active mode,
// as we are currently assuming that there is no code that alters the MCU
// activity state (supply state)
// TODO: Missing support for CONTROL and ISOCHRONOUS types
int init_usb_device(void) {
	set_enable_clk_ctrl(2, (1U << 16) | (1U << 17));
	int err = change_dcdc_voltage_lvl(0x02);
	if (err) return err;

	const uint8_t endpts_cnt = 2; //4;
	for (unsigned int i = 0; i < USB_MAX_HW_RESETS; ++i) {
		err = reset_usb_controller(endpts_cnt);
		if (err) return err;

		err = handle_bus_reset();
		if (err == 0) break;
	}

	init_endpoint(0, CONTROL);
	//init_endpoint(1, BULK);

	endpoint_transfer_descriptor_t* tdts = calloc(sizeof(endpoint_transfer_descriptor_t), endpts_cnt);
	if (tdts == NULL) {
		free(device_queue_heads);
		return -1;
	}

	const uint8_t* buf = calloc(1, 512 * endpts_cnt);
	if (buf == NULL) {
		free(device_queue_heads);
		free(tdts);
		return -1;
	}

	for (unsigned int i = 0; i < endpts_cnt; ++i) {
		init_transfer_descriptor(tdts, buf + 512 * i, 512);
		add_dtd_to_dqh(tdts + i, device_queue_heads + i);
	}

	return 0;
}

__attribute__((section(".after_vectors.usbhs_dcd_handler"), naked, noreturn))
void usbhs_dcd_handler(void) {
	__asm__ volatile("bkpt");
	__asm__ volatile("bx lr");
}

#endif //_USB_H_
