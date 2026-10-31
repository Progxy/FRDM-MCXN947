#ifndef _DEBUG_UART_H_
#define _DEBUG_UART_H_

#include <syscon.h>
#include <clock.h>
#include <port.h>
#include "./utils.h"

typedef enum { DEBUG_UART_OK = 0, DEBUG_UART_UNSUPPORTED = 1, DEBUG_UART_INIT_FAIL = 2, DEBUG_UART_TX_FAIL = 3, DEBUG_UART_RX_FAIL = 4 } DebugUARTErrors;

const unsigned int lp_flexcomm0_addr = 0x40092000;
const unsigned int lp_flexcomm1_addr = 0x40093000;
const unsigned int lp_flexcomm2_addr = 0x40094000;
const unsigned int lp_flexcomm3_addr = 0x40095000;
const unsigned int lp_flexcomm4_addr = 0x400B4000;
const unsigned int lp_flexcomm5_addr = 0x400B5000;
const unsigned int lp_flexcomm6_addr = 0x400B6000;
const unsigned int lp_flexcomm7_addr = 0x400B7000;
const unsigned int lp_flexcomm8_addr = 0x400B8000;
const unsigned int lp_flexcomm9_addr = 0x400B9000;

const unsigned int lp_uart0_addr = 0x40092000;
const unsigned int lp_uart1_addr = 0x40093000;
const unsigned int lp_uart2_addr = 0x40094000;
const unsigned int lp_uart3_addr = 0x40095000;
const unsigned int lp_uart4_addr = 0x400B4000;
const unsigned int lp_uart5_addr = 0x400B5000;
const unsigned int lp_uart6_addr = 0x400B6000;
const unsigned int lp_uart7_addr = 0x400B7000;
const unsigned int lp_uart8_addr = 0x400B8000;
const unsigned int lp_uart9_addr = 0x400B9000;

const unsigned int pselid_addr = 0xFF8;

typedef struct {
	uint8_t persel: 3;
	uint8_t lock: 1;
	uint8_t uart_present: 1;
	uint8_t spi_present: 1;
	uint8_t i2c_present: 1;
	uint8_t rsv: 1;
	uint8_t rsv1: 4;
	uint32_t id: 20;
} __attribute__((packed)) pselid_t;

typedef struct {
	uint8_t pt: 1;
	uint8_t pe: 1;
	uint8_t ilt: 1;
	uint8_t wake: 1;
	uint8_t m: 1;
	uint8_t rsrc: 1;
	uint8_t rsv: 1;
	uint8_t loops: 1;
	uint8_t idle_cfg: 3;
	uint8_t m7: 1;
	uint8_t rsv1: 2;
	uint8_t ma2ie: 1;
	uint8_t ma1ie: 1;
	uint8_t sbk: 1;
	uint8_t rwu: 1;
	uint8_t re: 1;
	uint8_t te: 1;
	uint8_t ilie: 1;
	uint8_t rie: 1;
	uint8_t tcie: 1;
	uint8_t tie: 1;
	uint8_t peie: 1;
	uint8_t feie: 1;
	uint8_t neie: 1;
	uint8_t orie: 1;
	uint8_t txinv: 1;
	uint8_t txdir: 1;
	uint8_t r9t8: 1;
	uint8_t r8t9: 1;
} __attribute__((packed)) uart_ctrl_t;

typedef struct {
	uint16_t sbr: 13;
	uint8_t sbns: 1;
	uint8_t rxedgie: 1;
	uint8_t lbkdie: 1;

	uint8_t resync_dis: 1;
	uint8_t bothedge: 1;
	uint8_t matcfg: 2;
	uint8_t ridmae: 1;
	uint8_t rdmae: 1;
	uint8_t rsv: 1;
	uint8_t tdmae: 1;

	uint8_t osr: 5;
	uint8_t m10: 1;
	uint8_t maen2: 1;
	uint8_t maen1: 1;
} __attribute__((packed)) uart_baud_t;

typedef struct {
	uint8_t rx_fifo_size: 3;
	uint8_t rxfe: 1;
	uint8_t tx_fifo_size: 3;
	uint8_t txfe: 1;
	uint8_t rxufe: 1;
	uint8_t txofe: 1;
	uint8_t rxiden: 3;
	uint8_t rsv: 1;
	uint8_t rx_flush: 1;
	uint8_t tx_flush: 1;
	uint8_t rxuf: 1;
	uint8_t txof: 1;
	uint8_t rsv1: 4;
	uint8_t rxempt: 1;
	uint8_t txempt: 1;
	uint8_t rsv2;
} __attribute__((packed)) uart_fifo_t;

typedef struct {
	uint16_t data: 10;
	uint8_t linbrk: 1;
	uint8_t idline: 1;
	uint8_t rxempt: 1;
	uint8_t fretsc: 1;
	uint8_t parity_err: 1;
	uint8_t noisy: 1;
	uint16_t rsv;
} __attribute__((packed)) uart_data_t;

typedef struct {
	uint8_t txwater: 3;
	uint8_t rsv: 5;
	uint8_t tx_count: 4;
	uint8_t rsv1: 4;
	uint8_t rxwater: 3;
	uint8_t rsv2: 5;
	uint8_t rx_count: 4;
	uint8_t rsv3: 4;
} __attribute__((packed)) uart_water_t;

typedef struct {
	uint8_t lbkfe: 1;
	uint8_t ame: 1;
	uint8_t rsv: 6;
	uint8_t msf: 1;
	uint8_t tsf: 1;
	uint8_t rsv1: 4;
	uint8_t ma2f: 1;
	uint8_t ma1f: 1;
	uint8_t pf: 1;
	uint8_t fe: 1;
	uint8_t nf: 1;
	uint8_t or: 1;
	uint8_t idle: 1;
	uint8_t rdrf: 1;
	uint8_t tc: 1;
	uint8_t tdre: 1;
	uint8_t raf: 1;
	uint8_t lbkde: 1;
	uint8_t brk13: 1;
	uint8_t rwuid: 1;
	uint8_t rxinv: 1;
	uint8_t msbf: 1;
	uint8_t rxedgif: 1;
	uint8_t lbkidf: 1;
} __attribute__((packed)) uart_stat_t;

typedef struct {
	uint8_t rsv: 1;
	uint8_t rst: 1;
	uint8_t rsv1: 6;
	uint32_t rsv2: 24;
} __attribute__((packed)) uart_global_t;

static_assert(sizeof(pselid_t)     == 4, "Size must be 4 bytes");
static_assert(sizeof(uart_ctrl_t)  == 4, "Size must be 4 bytes");
static_assert(sizeof(uart_baud_t)  == 4, "Size must be 4 bytes");
static_assert(sizeof(uart_fifo_t)  == 4, "Size must be 4 bytes");
static_assert(sizeof(uart_data_t)  == 4, "Size must be 4 bytes");
static_assert(sizeof(uart_water_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(uart_stat_t)  == 4, "Size must be 4 bytes");
static_assert(sizeof(uart_global_t)  == 4, "Size must be 4 bytes");

const unsigned int uart_global_addr = 0x08;
const unsigned int uart_baud_addr = 0x10;
const unsigned int uart_stat_addr = 0x14;
const unsigned int uart_ctrl_addr = 0x18;
const unsigned int uart_data_addr = 0x1C;
const unsigned int uart_fifo_addr = 0x28;
const unsigned int uart_water_addr = 0x2C;

typedef enum { FLEXCOMM_NONE = 0, FLEXCOMM_UART = 1, FLEXCOMM_SPI = 2, FLEXCOMM_I2C = 3, FLEXCOMM_UART_I2C = 7 } FlexCommPeripheralType;
typedef enum { UART_EVEN_PARITY = 0, UART_ODD_PARITY = 1} UARTParity;

static const int fifo_size[8] = { 1, 4, 8, 16, 32, 64, 128, 256 };

int init_debug_uart(const uint32_t baud_rate) {
	(void) baud_rate;

	int err = enable_sirc_clk();
	if (err) return -DEBUG_UART_INIT_FAIL;

	set_ahb_clk_ctrl(1, (1U << FC4));
	enable_flexcomm_clock(4, 2);

	err = set_pin_function(1, 8, (0x02U << 8));
	if (err) return -DEBUG_UART_INIT_FAIL;
	err = set_pin_function(1, 9, (0x02U << 8));
	if (err) return -DEBUG_UART_INIT_FAIL;

	volatile pselid_t* pselid = (pselid_t*) (lp_flexcomm4_addr + pselid_addr);
	if (pselid -> uart_present == 0) return -DEBUG_UART_UNSUPPORTED;

	if (pselid -> lock) return -DEBUG_UART_INIT_FAIL;
	pselid -> lock = 0;
	pselid -> persel = FLEXCOMM_UART;
	pselid -> lock = 1;

	// Reset the LPUART
	volatile uart_global_t* uart_global = (uart_global_t*) (lp_uart4_addr + uart_global_addr);
	uart_global -> rst = 1;
	uart_global -> rst = 0;

	volatile uart_ctrl_t* uart_ctrl = (uart_ctrl_t*) (lp_uart4_addr + uart_ctrl_addr);
	uart_ctrl -> re = 0;
	uart_ctrl -> te = 0;

	// Set the baud rate
	volatile uart_baud_t* uart_baud = (uart_baud_t*) (lp_uart4_addr + uart_baud_addr);
	// TODO: Find a good way to get the sbr and osr ideal values.
	//       Currently fixed baud rate of 115_200
	uart_baud -> osr = 0x07;
	uart_baud -> sbr = 13;

	// Enable FIFOs and set their sizes
	volatile uart_fifo_t* uart_fifo = (uart_fifo_t*) (lp_uart4_addr + uart_fifo_addr);
	volatile uart_water_t* uart_water = (uart_water_t*) (lp_uart4_addr + uart_water_addr);

	const int tx_fifo_size = fifo_size[uart_fifo -> tx_fifo_size];
	uart_water -> txwater = (tx_fifo_size & 0x07) - 1;

	uart_fifo -> txfe = 1;
	uart_fifo -> rxfe = 1;
	uart_fifo -> rx_flush = 1;
	uart_fifo -> tx_flush = 1;

	// Enable parity bit even/odd
	// TODO: extend bit payload size (e.g. 8 bits -> 9 bits, data + parity bit)
	// uart_ctrl -> pt = UART_ODD_PARITY; // UART_EVEN_PARITY;
	// uart_ctrl -> pe = 1;

	// Enable TX and RX
	uart_ctrl -> re = 1;
	uart_ctrl -> te = 1;

	return DEBUG_UART_OK;
}

int send_uart(const void* data, const unsigned int size) {
	volatile uart_fifo_t* uart_fifo = (uart_fifo_t*) (lp_uart4_addr + uart_fifo_addr);
	volatile uart_water_t* uart_water = (uart_water_t*) (lp_uart4_addr + uart_water_addr);
	volatile uart_stat_t* uart_stat = (uart_stat_t*) (lp_uart4_addr + uart_stat_addr);
	volatile uart_data_t* uart_data = (uart_data_t*) (lp_uart4_addr + uart_data_addr);

	for (unsigned int i = 0; i < size; ++i) {
		unsigned int cnt = 0;
		while (!uart_stat -> tdre) delay_us(1);
		uart_data -> data = ((uint8_t*) data)[i];
		if (uart_data -> fretsc || uart_data -> parity_err || uart_data -> noisy) return -DEBUG_UART_TX_FAIL;
	}

	while (!uart_stat -> tc) delay_us(1);

	return DEBUG_UART_OK;
}

int recv_uart(void* data, const unsigned int size) {
	(void) data;
	(void) size;
	// Poll STAT[RDRF] for incoming data (if STAT[OR] is 1 the FIFO is full and new incoming data is lost)
	// Read data from DATA

	return DEBUG_UART_OK;
}

#endif // _DEBUG_UART_H_
