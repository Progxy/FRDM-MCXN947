#ifndef _USB_H_
#define _USB_H_

#include <stdint.h>
#include "./utils.h"
#include "./alloc.h"
#include "./syscon.h"
#include "./power.h"

typedef struct __attribute__((packed)) {
	union {
		struct {
			const uint16_t rsv2: 15;
			const uint8_t ios: 1;
			const uint16_t max_packet_len: 11;
			const uint8_t rsv: 2;
			const uint8_t zlt: 1;
			const uint8_t mult: 2;

			uint32_t current_dtd_ptr;
			uint32_t next_dtd_ptr;

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
} endpoint_queue_head_t;

typedef struct __attribute__((packed)) {
	const uint8_t t: 1;
	const uint8_t rsv1: 4;
	const uint32_t next_link_ptr: 27;

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
} endpoint_transfer_descriptor_t;

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
	uint8_t cm: 2;
	uint8_t es: 1;
	uint8_t slom: 1;
	uint8_t sdis: 1;
	const uint32_t rsv: 27;
} __attribute__((packed)) usb_mode_t;

typedef enum { IDLE = 0, DEVICE = 2, HOST = 3 } USBMode;

static_assert(sizeof(usb_cmd_t) == 4,                       "Size must be 4 bytes");
static_assert(sizeof(usb_mode_t) == 4,                      "Size must be 4 bytes");
static_assert(sizeof(usb_status_t) == 4,                    "Size must be 4 bytes");
static_assert(sizeof(usb_int_enable_t) == 4,                "Size must be 4 bytes");
static_assert(sizeof(usb_endptlistaddr_t) == 4,             "Size must be 4 bytes");
static_assert(sizeof(endpoint_queue_head_t) == 64,          "Size must be 64 bytes");
static_assert(sizeof(endpoint_transfer_descriptor_t) == 28, "Size must be 28 bytes");

const unsigned int usb_addr =  0x4010B000;
const unsigned int usb_cmd_offset = 0x140;
const unsigned int usb_status_offset = 0x144;
const unsigned int usb_int_enable_offset = 0x148;
const unsigned int usb_endptlistaddr_offset = 0x158;
const unsigned int usb_mode_offset = 0x1A8;

void init_device_queue_heads(endpoint_queue_head_t* device_queue_heads) {
	// The even elements in the list of dQH's are used for receive endpoints (OUT/SETUP)
	// and the odd elements are used for transmit endpoints (IN/INTERRUPT)
	// One device queue head must be initialized for each active endpoint.
	// To initialize a device queue head:
	// • Write the wMaxPacketSize field as required by the USB Chapter 9 or application specific protocol.
	// • Write the multiplier field to 0 for control, bulk, and interrupt endpoints. For ISO endpoints, set the multiplier to 1,2, or 3 as
	// required bandwidth and in conjunction with the USB Chapter 9 protocol.
	// NOTE: In FS mode, the multiplier field can only be 1 for ISO endpoints.
	// • Write the next dTD Terminate bit field to 1.
	// • Write the Active bit in the status field to 0.
	// • Write the Halt bit in the status field to 0.
	// NOTE: The DCD must only modify dQH if the associated endpoint is not primed and there are no outstanding dTD's.

	memset(device_queue_heads, 0, 2 * sizeof(endpoint_queue_head_t));
	return;
}

int init_usb_device(void) {
	set_enable_clk_ctrl(2, (1U << 16) | (1U << 17));

	// TODO: Theoretically should ensure that the MCU is running in active mode,
	// as we are currently assuming that there is no code that alters the MCU
	// activity state (supply state)

	int err = change_dcdc_voltage_lvl(0x02);
	if (err) return err;

	volatile usb_cmd_t* usb_cmd = (usb_cmd_t*) (usb_addr + usb_cmd_offset);
	usb_cmd -> rs = 0;

	volatile usb_status_t* usb_status = (usb_status_t*) (usb_addr + usb_status_offset);
	while (!usb_status -> hch) delay_ms(5);

	usb_cmd -> rst = 1;
	while (usb_cmd -> rst) delay_ms(5);

	volatile usb_mode_t* usb_mode = (usb_mode_t*) (usb_addr + usb_mode_offset);
	usb_mode -> cm = DEVICE;

	endpoint_queue_head_t* device_queue_heads = calloc(sizeof(endpoint_queue_head_t), 2);
	if (device_queue_heads == NULL) return -1;

	// Initialize device queue heads 0 Tx & 0 Rx.
	init_device_queue_heads(device_queue_heads);

	volatile usb_endptlistaddr_t* usb_endptlistaddr = (usb_endptlistaddr_t*) (usb_addr + usb_endptlistaddr_offset);
	usb_endptlistaddr -> epbase = mask_lower_bits((unsigned int) device_queue_heads, 11);

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

__attribute__((section(".after_vectors.usbhs_dcd_handler"), naked, noreturn))
void usbhs_dcd_handler(void) {
	__asm__ volatile("bkpt");
	__asm__ volatile("bx lr");
}

#endif //_USB_H_
