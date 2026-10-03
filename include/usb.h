#ifndef _USB_H_
#define _USB_H_

#include <stdint.h>
#include "./alloc.h"
#include "./syscon.h"

typedef struct __attribute__((packed)) {
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

static_assert(sizeof(endpoint_queue_head_t)          == 48, "Size must be 48 bytes");
static_assert(sizeof(endpoint_transfer_descriptor_t) == 28, "Size must be 28 bytes");

void init_usb_device(void) {
	set_enable_clk_ctrl(2, (1U << 16) | (1U << 17));

	// Configure SPC.ACTIVE_CFG[DCDC_VDD_LVL] =
	// SPC.ACTIVE_CFG[CORELDO_VDD_LVL] >= 0x2 for correct operation of the module.

	// Set controller mode in USB.USBMODE register
	// NOTE: Transitioning from host mode to device mode requires a device controller reset before modifying USB.USBMODE.

	// Allocate and Initialize device queue heads in system memory.
	// Initialize device queue heads 0 Tx & 0 Rx.

	// Configure USB.ENDPOINTLISTADDR Pointer.
	// Allocate 32 elements endpoint queue head list: USB.ENDPOINTLISTADDR
	// Allocate the In and Out dQH, where each dQH must be aligned on 64-byte boundaries

	// Enable the microprocessor interrupt associated with the USB core.
	//— Recommended: enable all device interrupts including: USBINT, USBERRINT, Port Change Detect, USB Reset Received, DCSuspend.

	// Set Run/Stop bit to Run Mode.

	return;
}

#endif //_USB_H_
