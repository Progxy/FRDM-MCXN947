#ifndef _ENET_H_
#define _ENET_H_

#include <utils.h>
#include <syscon.h>
#include <clock.h>
#include <port.h>
#include <alloc.h>
#include <nvic.h>

typedef enum { ENET_OK = 0, ENET_INIT_FAIL, } ENETStatus;
typedef enum { ENET_MII_PHY = 0, ENET_RMII_PHY = 1 } ENETInterfaceType;
typedef enum { ENET_PBL1 = 1, ENET_PBL2 = 2, ENET_PBL4 = 4, ENET_PBL8 = 8, ENET_PBL16 = 16, ENET_PBL32 = 32 } ENETBurstLenghts;

const uint16_t mac_address_high = 0x0004;
const uint32_t mac_address_low  = 0x9F08FE0B;

const unsigned int enet0_addr = 0x40100000;
const unsigned int dma_mode_reg_addr = 0x1000;
const unsigned int dma_sysbus_mode_reg_addr = 0x1004;
const unsigned int dma_ch0_rx_ctrl2_reg_addr = 0x1130;
const unsigned int dma_ch1_rx_ctrl2_reg_addr = 0x11B0;
const unsigned int dma_ch0_tx_rbl_reg_addr = 0x112C;
const unsigned int dma_ch1_tx_rbl_reg_addr = 0x11AC;
const unsigned int dma_ch0_tx_addr_reg_addr = 0x1114;
const unsigned int dma_ch0_rx_addr_reg_addr = 0x111C;
const unsigned int dma_ch1_tx_addr_reg_addr = 0x1194;
const unsigned int dma_ch1_rx_addr_reg_addr = 0x119C;
const unsigned int dma_ch0_tx_tail_ptr_reg_addr = 0x1120;
const unsigned int dma_ch0_rx_tail_ptr_reg_addr = 0x1128;
const unsigned int dma_ch1_tx_tail_ptr_reg_addr = 0x11A0;
const unsigned int dma_ch1_rx_tail_ptr_reg_addr = 0x11A8;
const unsigned int dma_ch0_ctrl_reg_addr    = 0x1100;
const unsigned int dma_ch0_tx_ctrl_reg_addr = 0x1104;
const unsigned int dma_ch0_rx_ctrl_reg_addr = 0x1108;
const unsigned int dma_ch1_ctrl_reg_addr    = 0x1180;
const unsigned int dma_ch1_tx_ctrl_reg_addr = 0x1184;
const unsigned int dma_ch1_rx_ctrl_reg_addr = 0x1188;
const unsigned int dma_ch0_inte_reg_addr = 0x1134;
const unsigned int dma_ch1_inte_reg_addr = 0x11B4;
const unsigned int mtl_op_mode_reg_addr = 0xC00;
const unsigned int mtl_txq0_op_mode_reg_addr = 0xD00;
const unsigned int mtl_txq1_op_mode_reg_addr = 0xD40;
const unsigned int mtl_rxq0_op_mode_reg_addr = 0xD30;
const unsigned int mtl_rxq1_op_mode_reg_addr = 0xD70;
const unsigned int mac_address0_high_addr = 0x300;
const unsigned int mac_address0_low_addr = 0x304;
const unsigned int mac_packet_filter_reg_addr = 0x08;
const unsigned int mac_q0_tx_fctrl_reg_addr = 0x70;
const unsigned int mac_inte_reg_addr = 0xB4;
const unsigned int mac_int_status_reg_addr = 0xB0;
const unsigned int mac_config_reg_addr = 0x00;
const unsigned int mtl_int_status_reg_addr = 0xC20;
const unsigned int dma_int_status_reg_addr = 0x1008;
const unsigned int dma_debug_status0_reg_addr = 0x100C;
const unsigned int dma_status_ch0_reg_addr = 0x1160;
const unsigned int dma_status_ch1_reg_addr = 0x11E0;
const unsigned int mac_debug_reg_addr = 0x114;
const unsigned int mtl_rxq0_debug_reg_addr = 0xD38;
const unsigned int mtl_rxq1_debug_reg_addr = 0xD78;
const unsigned int mac_mdio_address_reg_addr = 0x200;

typedef struct {
	uint8_t swr: 1;
	uint8_t da: 1;
	uint8_t taa: 3;
	uint8_t rsv1: 3;
	uint8_t rsv2: 3;
	uint8_t txpr: 1;
	uint8_t pr: 3;
	uint8_t rsv3: 1;
	uint16_t rsv4;
} __attribute__((packed)) dma_mode_t;

typedef struct {
	uint8_t fb: 1;
	uint8_t rsv1: 7;
	uint8_t rsv2: 4;
	uint8_t aal: 1;
	uint8_t rsv3: 1;
	uint8_t mb: 1;
	uint8_t rb: 1;
	uint16_t rsv4;
} __attribute__((packed)) dma_sysbus_mode_t;

typedef struct {
	uint32_t rdrl: 10;
	uint8_t rsv: 6;
	uint8_t arbs;
	uint8_t rsv1;
} __attribute__((packed)) dma_ch_rx_ctrl2_t;

typedef struct {
	uint16_t rsv;
	uint8_t pblx8: 1;
	uint8_t rsv1: 1;
	uint8_t dsl: 3;
	uint8_t rsv2: 3;
	uint8_t rsv3;
} __attribute__((packed)) dma_ch_ctrl_t;

typedef struct {
	uint8_t st: 1;
	uint8_t tcw: 3;
	uint8_t osf: 1;
	uint8_t rsv: 3;
	uint8_t rsv1;
	uint8_t txpbl: 6;
	uint8_t etic: 1;
	uint8_t rsv2: 1;
	uint8_t rsv3;
} __attribute__((packed)) dma_ch_tx_ctrl_t;

typedef struct {
	uint8_t tie: 1;
	uint8_t txse: 1;
	uint8_t tbue: 1;
	uint8_t rsv: 3;
	uint8_t rie: 1;
	uint8_t rbue: 1;
	uint8_t rse: 1;
	uint8_t rwte: 1;
	uint8_t etie: 1;
	uint8_t erie: 1;
	uint8_t fbee: 1;
	uint8_t cdee: 1;
	uint8_t aie: 1;
	uint8_t nie: 1;
	uint16_t rsv1;
} __attribute__((packed)) dma_ch_inte_t;

typedef struct {
	uint8_t ti: 1;
	uint8_t tps: 1;
	uint8_t tbu: 1;
	uint8_t rsv: 3;
	uint8_t ri: 1;
	uint8_t rbu: 1;
	uint8_t rps: 1;
	uint8_t rwt: 1;
	uint8_t eti: 1;
	uint8_t eri: 1;
	uint8_t fbe: 1;
	uint8_t cde: 1;
	uint8_t ais: 1;
	uint8_t nis: 1;
	uint8_t teb: 3;
	uint8_t reb: 3;
	uint8_t rsv1: 2;
	uint8_t rsv2;
} __attribute__((packed)) dma_ch_int_status_t;

typedef struct {
	uint8_t sr: 1;
	uint16_t rbsz: 14;
	uint8_t rsv: 1;
	uint8_t rxpbl: 6;
	uint8_t eric: 1;
	uint8_t rsv1: 1;
	uint8_t rsv2: 7;
	uint8_t rpf: 1;
} __attribute__((packed)) dma_ch_rx_ctrl_t;

typedef struct {
	uint32_t buf1_ap;
	uint32_t buf2_ap;
	uint16_t b1l: 14;
	uint8_t rsv: 2;
	uint16_t b2l: 14;
	uint8_t ttse: 1;
	uint8_t ioc: 1;
	uint16_t fl: 15;
	uint8_t rsv1: 1;
	uint8_t cic: 2;
	uint8_t rsv2: 1;
	uint8_t slot_num: 4;
	uint8_t saic_low: 1;
	uint8_t saic_high: 2;
	uint8_t cpc: 2;
	uint8_t ld: 1;
	uint8_t fd: 1;
	uint8_t ctxt: 1;
	uint8_t own: 1;
} __attribute__((packed)) dma_rf_tdes_t;

typedef struct {
	uint32_t ttsl;
	uint32_t ttsh;
	uint32_t rsv;
	uint8_t ihe: 1;
	uint8_t db: 1;
	uint8_t uf: 1;
	uint8_t ed: 1;
	uint8_t cc: 4;
	uint8_t ec: 1;
	uint8_t lc: 1;
	uint8_t nc: 1;
	uint8_t loc: 1;
	uint8_t pce: 1;
	uint8_t ff: 1;
	uint8_t jt: 1;
	uint8_t es: 1;
	uint8_t rsv1: 1;
	uint8_t ttss: 1;
	uint16_t rsv2: 10;
	uint8_t ld: 1;
	uint8_t fd1: 1;
	uint8_t ctxt: 1;
	uint8_t own: 1;
} __attribute__((packed)) dma_wbf_tdes_t;

typedef struct {
	uint32_t buf1_ap;
	uint32_t rsv;
	uint32_t buf2_ap;
	uint32_t rsv1: 24;
	uint8_t buf1v: 1;
	uint8_t buf2v: 1;
	uint8_t rsv2: 4;
	uint8_t ioc: 1;
	uint8_t own: 1;
} __attribute__((packed)) dma_rf_rdes_t;

typedef struct {
	uint16_t ovt;
	uint16_t ivt;
	uint8_t pt: 3;
	uint8_t iphe: 1;
	uint8_t ipv4: 1;
	uint8_t ipv6: 1;
	uint8_t ipcb: 1;
	uint8_t ipce: 1;
	uint8_t pmt: 4;
	uint8_t pft: 1;
	uint8_t pv: 1;
	uint8_t tsa: 1;
	uint8_t td: 1;
	uint16_t opc;
	uint16_t rsv;
	uint8_t saf: 1;
	uint8_t daf: 1;
	uint8_t rsv1: 1;
	uint8_t madrm_low: 5;
	uint8_t madrm_high: 3;
	uint8_t rsv3: 5;
	uint16_t pl: 15;
	uint8_t es: 1;
	uint8_t lt: 3;
	uint8_t de: 1;
	uint8_t re: 1;
	uint8_t oe: 1;
	uint8_t rwt: 1;
	uint8_t gp: 1;
	uint8_t ce: 1;
	uint8_t rs0v: 1;
	uint8_t rs1v: 1;
	uint8_t rs2v: 1;
	uint8_t ld: 1;
	uint8_t fd: 1;
	uint8_t ctxt: 1;
	uint8_t own: 1;
} __attribute__((packed)) dma_wbf_rdes_t;

typedef struct {
	uint8_t rsv: 1;
	uint8_t dtxsts: 1;
	uint8_t raa: 1;
	uint8_t rsv2: 2;
	uint8_t schalg: 2;
	uint8_t rsv3: 1;
	uint8_t cntp_rst: 1;
	uint8_t cnt_clr: 1;
	uint8_t rsv4: 6;
	uint16_t rsv5;
} __attribute__((packed)) mtl_op_mode_t;

typedef struct {
	uint8_t ftq: 1;
	uint8_t tsf: 1;
	uint8_t txqen: 2;
	uint8_t ttc: 3;
	uint8_t rsv: 1;
	uint8_t rsv1;
	uint8_t tqs: 3;
	uint16_t rsv2: 13;
} __attribute__((packed)) mtl_txq_op_mode_t;

typedef struct {
	uint8_t rtc: 2;
	uint8_t rsv: 1;
	uint8_t fup: 1;
	uint8_t fep: 1;
	uint8_t rsf: 1;
	uint8_t dis_tcp_ef: 1;
	uint8_t rsv1: 1;
	uint8_t rsv2;
	uint8_t rsv3: 4;
	uint8_t rqs: 3;
	uint8_t rsv4: 1;
	uint8_t rsv5;
} __attribute__((packed)) mtl_rxq_op_mode_t;

typedef struct {
	uint8_t pr: 1;
	uint8_t rsv: 2;
	uint8_t daif: 1;
	uint8_t pm: 1;
	uint8_t dbf: 1;
	uint8_t pcf: 2;
	uint8_t rsv1;
	uint8_t vtfe: 1;
	uint16_t rsv2: 14;
	uint8_t ra: 1;
} __attribute__((packed)) mac_packet_filter_t;

typedef struct {
	uint8_t fcb_bpa: 1;
	uint8_t tfe: 1;
	uint8_t rsv: 2;
	uint8_t plt: 3;
	uint8_t dzpq: 1;
	uint8_t rsv1;
	uint16_t pt;
} __attribute__((packed)) mac_qtx_fctrl_t;

typedef struct {
	uint8_t rsv: 3;
	uint8_t phyie: 1;
	uint8_t pmtie: 1;
	uint8_t lpiie: 1;
	uint8_t rsv1: 2;
	uint8_t rsv2: 4;
	uint8_t tsie: 1;
	uint8_t txstsie: 1;
	uint8_t rxstsie: 1;
	uint8_t rsv3: 1;
	uint8_t rsv4: 2;
	uint8_t mdioie: 1;
	uint16_t rsv5: 13;
} __attribute__((packed)) mac_inte_t;

typedef struct {
	uint8_t re: 1;
	uint8_t te: 1;
	uint8_t prelen: 2;
	uint8_t dc: 1;
	uint8_t bl: 2;
	uint8_t rsv: 1;
	uint8_t dr: 1;
	uint8_t dcrs: 1;
	uint8_t do_: 1;
	uint8_t ecrsfd: 1;
	uint8_t lm: 1;
	uint8_t dm: 1;
	uint8_t fes: 1;
	uint8_t ps: 1;
	uint8_t je: 1;
	uint8_t jd: 1;
	uint8_t rsv1: 1;
	uint8_t wd: 1;
	uint8_t acs: 1;
	uint8_t cst: 1;
	uint8_t s2kp: 1;
	uint8_t gpslce: 1;
	uint8_t ipg: 3;
	uint8_t ipc: 1;
	uint8_t sarc: 3;
	uint8_t rsv2: 1;
} __attribute__((packed)) mac_config_t;

typedef struct {
	uint8_t gb: 1;
	uint8_t c45e: 1;
	uint8_t goc_0: 1;
	uint8_t goc_1: 1;
	uint8_t skap: 1;
	uint8_t rsv: 3;
	uint8_t cr: 4;
	uint8_t ntc: 3;
	uint8_t rsv1: 1;
	uint8_t rda: 5;
	uint8_t pa_low: 3;
	uint8_t pa_high: 2;
	uint8_t btb: 1;
	uint8_t pse: 1;
	uint8_t rsv2: 4;
} __attribute__((packed)) mac_mdio_address_t;

typedef struct {
	uint8_t dc0_is: 1;
	uint8_t dc1_is: 1;
	uint16_t rsv: 14;
	uint8_t mtl_is: 1;
	uint8_t mac_is: 1;
	uint16_t rsv1: 14;
} __attribute__((packed)) dma_int_status_t;

static_assert(sizeof(dma_int_status_t)    == 4,  "Size must be 4 bytes");
static_assert(sizeof(mac_mdio_address_t)  == 4,  "Size must be 4 bytes");
static_assert(sizeof(dma_ch_int_status_t) == 4,  "Size must be 4 bytes");
static_assert(sizeof(mac_inte_t)          == 4,  "Size must be 4 bytes");
static_assert(sizeof(mac_qtx_fctrl_t)     == 4,  "Size must be 4 bytes");
static_assert(sizeof(mac_packet_filter_t) == 4,  "Size must be 4 bytes");
static_assert(sizeof(mtl_op_mode_t)       == 4,  "Size must be 4 bytes");
static_assert(sizeof(mtl_txq_op_mode_t)   == 4,  "Size must be 4 bytes");
static_assert(sizeof(mtl_rxq_op_mode_t)   == 4,  "Size must be 4 bytes");
static_assert(sizeof(dma_ch_inte_t)       == 4,  "Size must be 4 bytes");
static_assert(sizeof(dma_ch_ctrl_t)       == 4,  "Size must be 4 bytes");
static_assert(sizeof(dma_ch_tx_ctrl_t)    == 4,  "Size must be 4 bytes");
static_assert(sizeof(dma_ch_rx_ctrl_t)    == 4,  "Size must be 4 bytes");
static_assert(sizeof(dma_mode_t)          == 4,  "Size must be 4 bytes");
static_assert(sizeof(dma_sysbus_mode_t)   == 4,  "Size must be 4 bytes");
static_assert(sizeof(dma_ch_rx_ctrl2_t)   == 4,  "Size must be 16 bytes");
static_assert(sizeof(dma_rf_tdes_t)       == 16, "Size must be 16 bytes");
static_assert(sizeof(dma_wbf_tdes_t)      == 16, "Size must be 16 bytes");
static_assert(sizeof(dma_rf_rdes_t)       == 16, "Size must be 16 bytes");
static_assert(sizeof(dma_wbf_rdes_t)      == 16, "Size must be 16 bytes");

// NOTE: After a write operation, when a register content is being transferred to a different clock domain, user needs to wait at
// least for four cycles of the destination clock (PHY Receive clock, PHY Transmit clock, or PTP clock) before writing
// to the same register.

#define FLOW_CTRL_PAUSE_TIME 4096
#define RECEIVE_BUFFER_SIZE 8192
#define DMA_DESC_RB_SIZE 10
__attribute__((aligned(4))) dma_rf_tdes_t ch0_tx_desc_rb[DMA_DESC_RB_SIZE] = {0};
__attribute__((aligned(4))) dma_rf_rdes_t ch0_rx_desc_rb[DMA_DESC_RB_SIZE] = {0};
__attribute__((aligned(4))) dma_rf_tdes_t ch1_tx_desc_rb[DMA_DESC_RB_SIZE] = {0};
__attribute__((aligned(4))) dma_rf_rdes_t ch1_rx_desc_rb[DMA_DESC_RB_SIZE] = {0};

__attribute__((aligned(4))) uint8_t ch0_rx_bufs[DMA_DESC_RB_SIZE * 2 * RECEIVE_BUFFER_SIZE] = {0};
__attribute__((aligned(4))) uint8_t ch1_rx_bufs[DMA_DESC_RB_SIZE * 2 * RECEIVE_BUFFER_SIZE] = {0};

int init_enet_dma(void) {
	volatile uint32_t* dma_mode_reg = (uint32_t*) (enet0_addr + dma_mode_reg_addr);
	dma_mode_t dma_mode = *(dma_mode_t*)dma_mode_reg;
	//if (dma_mode.swr) __asm__("bkpt");
	dma_mode.swr = 1;
	*dma_mode_reg = *((uint32_t*) &dma_mode);

	delay_us(100); // Delay consecutive write/read to the same register, as the sysclock and the enet0 clock are on different domains
	while (*dma_mode_reg & 0x01) delay_us(5);

	volatile uint32_t* dma_sysbus_mode_reg = (uint32_t*) (enet0_addr + dma_sysbus_mode_reg_addr);
	dma_sysbus_mode_t dma_sysbus_mode = *(dma_sysbus_mode_t*)dma_sysbus_mode_reg;
	dma_sysbus_mode.fb  = 1;  // Fixed Burst Length
	dma_sysbus_mode.aal = 1; // Address-Aligned burst transfer
	dma_sysbus_mode.mb  = 0;  // Mixed Burst (fixed under 16 bytes, undefined length burst otherwise)
	dma_sysbus_mode.rb  = 0;  // Rebuild INCRx Burst
	*dma_sysbus_mode_reg = *((uint32_t*) &dma_sysbus_mode);

	memset(ch0_tx_desc_rb, 0, sizeof(dma_rf_tdes_t) * DMA_DESC_RB_SIZE);
	memset(ch0_rx_desc_rb, 0, sizeof(dma_rf_rdes_t) * DMA_DESC_RB_SIZE);
	memset(ch1_tx_desc_rb, 0, sizeof(dma_rf_tdes_t) * DMA_DESC_RB_SIZE);
	memset(ch1_rx_desc_rb, 0, sizeof(dma_rf_rdes_t) * DMA_DESC_RB_SIZE);
	memset(ch0_rx_bufs, 0, sizeof(ch0_rx_bufs));
	memset(ch1_rx_bufs, 0, sizeof(ch1_rx_bufs));
	for (unsigned int i = 0; i < DMA_DESC_RB_SIZE - 1; ++i) {
		ch0_rx_desc_rb[i].buf1_ap = (uint32_t) ch0_rx_bufs + RECEIVE_BUFFER_SIZE * i * 2;
		ch0_rx_desc_rb[i].buf2_ap = (uint32_t) ch0_rx_bufs + RECEIVE_BUFFER_SIZE * i * 2 + RECEIVE_BUFFER_SIZE;
		ch0_rx_desc_rb[i].buf1v = 1;
		ch0_rx_desc_rb[i].buf2v = 1;
		ch0_rx_desc_rb[i].ioc = 1;
		ch0_rx_desc_rb[i].own = 1;
		ch1_rx_desc_rb[i].buf1_ap = (uint32_t) ch1_rx_bufs + RECEIVE_BUFFER_SIZE * i * 2;
		ch1_rx_desc_rb[i].buf2_ap = (uint32_t) ch1_rx_bufs + RECEIVE_BUFFER_SIZE * i * 2 + RECEIVE_BUFFER_SIZE;
		ch1_rx_desc_rb[i].buf1v = 1;
		ch1_rx_desc_rb[i].buf2v = 1;
		ch1_rx_desc_rb[i].ioc = 1;
		ch1_rx_desc_rb[i].own = 1;
		ch0_tx_desc_rb[i].own = 1;
		ch1_tx_desc_rb[i].own = 1;
	}

	dma_ch_rx_ctrl2_t dma_ch_rx_ctrl2 = {0};
	dma_ch_rx_ctrl2.rdrl = DMA_DESC_RB_SIZE - 1;
	dma_ch_rx_ctrl2.arbs = 0;

	volatile uint32_t* dma_ch0_rx_ctrl2_reg = (uint32_t*) (enet0_addr + dma_ch0_rx_ctrl2_reg_addr);
	*dma_ch0_rx_ctrl2_reg = *((uint32_t*) &dma_ch_rx_ctrl2);
	volatile uint32_t* dma_ch1_rx_ctrl2_reg = (uint32_t*) (enet0_addr + dma_ch1_rx_ctrl2_reg_addr);
	*dma_ch1_rx_ctrl2_reg = *((uint32_t*) &dma_ch_rx_ctrl2);

	volatile uint32_t* dma_ch0_tx_rbl = (uint32_t*) (enet0_addr + dma_ch0_tx_rbl_reg_addr);
	*dma_ch0_tx_rbl = (DMA_DESC_RB_SIZE - 1) & 0x3FF;
	volatile uint32_t* dma_ch1_tx_rbl = (uint32_t*) (enet0_addr + dma_ch1_tx_rbl_reg_addr);
	*dma_ch1_tx_rbl = (DMA_DESC_RB_SIZE - 1) & 0x3FF;

	volatile uint32_t* dma_ch0_tx_addr = (uint32_t*) (enet0_addr + dma_ch0_tx_addr_reg_addr);
	volatile uint32_t* dma_ch1_tx_addr = (uint32_t*) (enet0_addr + dma_ch1_tx_addr_reg_addr);
	*dma_ch0_tx_addr = ((uint32_t) ch0_tx_desc_rb) & 0xFFFFFFFC;
	*dma_ch1_tx_addr = ((uint32_t) ch1_tx_desc_rb) & 0xFFFFFFFC;

	volatile uint32_t* dma_ch0_rx_addr = (uint32_t*) (enet0_addr + dma_ch0_rx_addr_reg_addr);
	volatile uint32_t* dma_ch1_rx_addr = (uint32_t*) (enet0_addr + dma_ch1_rx_addr_reg_addr);
	*dma_ch0_rx_addr = ((uint32_t) ch0_rx_desc_rb) & 0xFFFFFFFC;
	*dma_ch1_rx_addr = ((uint32_t) ch1_rx_desc_rb) & 0xFFFFFFFC;

	volatile uint32_t* dma_ch0_tx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch0_tx_tail_ptr_reg_addr);
	volatile uint32_t* dma_ch1_tx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch1_tx_tail_ptr_reg_addr);
	*dma_ch0_tx_tail_ptr = ((uint32_t) ch0_tx_desc_rb) & 0xFFFFFFFC;
	*dma_ch1_tx_tail_ptr = ((uint32_t) ch1_tx_desc_rb) & 0xFFFFFFFC;

	volatile uint32_t* dma_ch0_rx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch0_rx_tail_ptr_reg_addr);
	volatile uint32_t* dma_ch1_rx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch1_rx_tail_ptr_reg_addr);
	*dma_ch0_rx_tail_ptr = ((uint32_t) (ch0_rx_desc_rb + DMA_DESC_RB_SIZE - 1)) & 0xFFFFFFFC;
	*dma_ch1_rx_tail_ptr = ((uint32_t) (ch1_rx_desc_rb + DMA_DESC_RB_SIZE - 1)) & 0xFFFFFFFC;

	dma_ch_ctrl_t dma_ch_ctrl = {0};
	dma_ch_ctrl.pblx8 = 1;
	dma_ch_ctrl.dsl = 0;

	volatile uint32_t* dma_ch0_ctrl_reg = (uint32_t*) (enet0_addr + dma_ch0_ctrl_reg_addr);
	*dma_ch0_ctrl_reg = *((uint32_t*) &dma_ch_ctrl);
	volatile uint32_t* dma_ch1_ctrl_reg = (uint32_t*) (enet0_addr + dma_ch1_ctrl_reg_addr);
	*dma_ch1_ctrl_reg = *((uint32_t*) &dma_ch_ctrl);

	dma_ch_tx_ctrl_t dma_ch_tx_ctrl = {0};
	dma_ch_tx_ctrl.st = 0;
	dma_ch_tx_ctrl.osf = 0;
	dma_ch_tx_ctrl.txpbl = ENET_PBL32;

	volatile uint32_t* dma_ch0_tx_ctrl_reg = (uint32_t*) (enet0_addr + dma_ch0_tx_ctrl_reg_addr);
	*dma_ch0_tx_ctrl_reg = *((uint32_t*) &dma_ch_tx_ctrl);
	volatile uint32_t* dma_ch1_tx_ctrl_reg = (uint32_t*) (enet0_addr + dma_ch1_tx_ctrl_reg_addr);
	*dma_ch1_tx_ctrl_reg = *((uint32_t*) &dma_ch_tx_ctrl);

	dma_ch_rx_ctrl_t dma_ch_rx_ctrl = {0};
	dma_ch_rx_ctrl.sr = 0;
	dma_ch_rx_ctrl.rxpbl = ENET_PBL32;
	dma_ch_rx_ctrl.rbsz = RECEIVE_BUFFER_SIZE;

	volatile uint32_t* dma_ch0_rx_ctrl_reg = (uint32_t*) (enet0_addr + dma_ch0_rx_ctrl_reg_addr);
	*dma_ch0_rx_ctrl_reg = *((uint32_t*) &dma_ch_rx_ctrl);
	volatile uint32_t* dma_ch1_rx_ctrl_reg = (uint32_t*) (enet0_addr + dma_ch1_rx_ctrl_reg_addr);
	*dma_ch1_rx_ctrl_reg = *((uint32_t*) &dma_ch_rx_ctrl);

	dma_ch_inte_t dma_ch_inte = {0};
	dma_ch_inte.tie = 1;
	dma_ch_inte.txse = 1;
	dma_ch_inte.tbue = 1;
	dma_ch_inte.rie = 1;
	dma_ch_inte.rbue = 1;
	dma_ch_inte.rse = 1;
	dma_ch_inte.rwte = 1;
	dma_ch_inte.fbee = 1;
	dma_ch_inte.cdee = 1;
	dma_ch_inte.aie = 1;
	dma_ch_inte.nie = 1;

	volatile uint32_t* dma_ch0_inte_reg = (uint32_t*) (enet0_addr + dma_ch0_inte_reg_addr);
	*dma_ch0_inte_reg = *((uint32_t*) &dma_ch_inte);
	volatile uint32_t* dma_ch1_inte_reg = (uint32_t*) (enet0_addr + dma_ch1_inte_reg_addr);
	*dma_ch1_inte_reg = *((uint32_t*) &dma_ch_inte);

	delay_us(100); // Delay consecutive write to the same register, as the sysclock and the enet0 clock are on different domains

	dma_ch_tx_ctrl.st = 1;
	*dma_ch0_tx_ctrl_reg = *((uint32_t*) &dma_ch_tx_ctrl);
	*dma_ch1_tx_ctrl_reg = *((uint32_t*) &dma_ch_tx_ctrl);

	dma_ch_rx_ctrl.sr = 1;
	*dma_ch0_rx_ctrl_reg = *((uint32_t*) &dma_ch_rx_ctrl);
	*dma_ch1_rx_ctrl_reg = *((uint32_t*) &dma_ch_rx_ctrl);

	return ENET_OK;
}

int init_enet_mtl_regs(void) {
	volatile uint32_t* mtl_op_mode_reg = (uint32_t*) (enet0_addr + mtl_op_mode_reg_addr);
	mtl_op_mode_t mtl_op_mode = {0};
	mtl_op_mode.raa = 0; // 1
	mtl_op_mode.schalg = 0x03; // 0x00
	*mtl_op_mode_reg = *((uint32_t*) &mtl_op_mode);

	// NOTE: using one-on-one mapping for rx queue mapping (e.g. mtl queue 0 -> dma channel 0)
	mtl_txq_op_mode_t mtl_txq_op_mode = {0};
	mtl_txq_op_mode.tsf = 1;
	mtl_txq_op_mode.txqen = 0x02;
	mtl_txq_op_mode.tqs = 0x7;
	volatile uint32_t* mtl_txq0_op_mode_reg = (uint32_t*) (enet0_addr + mtl_txq0_op_mode_reg_addr);
	*mtl_txq0_op_mode_reg = *((uint32_t*) &mtl_txq_op_mode);
	volatile uint32_t* mtl_txq1_op_mode_reg = (uint32_t*) (enet0_addr + mtl_txq1_op_mode_reg_addr);
	*mtl_txq1_op_mode_reg = *((uint32_t*) &mtl_txq_op_mode);

	mtl_rxq_op_mode_t mtl_rxq_op_mode = {0};
	mtl_rxq_op_mode.rsf = 1;
	mtl_rxq_op_mode.rqs = 0x7;
	volatile uint32_t* mtl_rxq0_op_mode_reg = (uint32_t*) (enet0_addr + mtl_rxq0_op_mode_reg_addr);
	*mtl_rxq0_op_mode_reg = *((uint32_t*) &mtl_rxq_op_mode);
	volatile uint32_t* mtl_rxq1_op_mode_reg = (uint32_t*) (enet0_addr + mtl_rxq1_op_mode_reg_addr);
	*mtl_rxq1_op_mode_reg = *((uint32_t*) &mtl_rxq_op_mode);

	return ENET_OK;
}

int init_enet_mac(void) {
	volatile uint32_t* mac_address0_high = (uint32_t*) (enet0_addr + mac_address0_high_addr);
	*mac_address0_high = (1U << 31) | mac_address_high;

	volatile uint32_t* mac_address0_low = (uint32_t*) (enet0_addr + mac_address0_low_addr);
	*mac_address0_low = mac_address_low;

	mac_packet_filter_t mac_packet_filter = {0};
	mac_packet_filter.pcf = 0x03;
	volatile uint32_t* mac_packet_filter_reg = (uint32_t*) (enet0_addr + mac_packet_filter_reg_addr);
	*mac_packet_filter_reg = *((uint32_t*) &mac_packet_filter);

	mac_qtx_fctrl_t mac_qtx_fctrl = {0};
	mac_qtx_fctrl.tfe = 1;
	mac_qtx_fctrl.pt = FLOW_CTRL_PAUSE_TIME; // Pause Time
	volatile uint32_t* mac_q0_tx_fctrl_reg = (uint32_t*) (enet0_addr + mac_q0_tx_fctrl_reg_addr);
	*mac_q0_tx_fctrl_reg = *((uint32_t*) &mac_qtx_fctrl);

	mac_inte_t mac_inte = {0};
	mac_inte.phyie = 1;
	mac_inte.pmtie = 1;
	mac_inte.tsie = 1;
	mac_inte.txstsie = 1;
	mac_inte.rxstsie = 1;
	mac_inte.mdioie = 1;
	volatile uint32_t* mac_inte_reg = (uint32_t*) (enet0_addr + mac_inte_reg_addr);
	*mac_inte_reg = *((uint32_t*) &mac_inte);

	mac_config_t mac_config = { .ps = 1 };
	mac_config.prelen = 0x02;
	mac_config.dc = 1;
	mac_config.ecrsfd = 0; // 1;
	mac_config.lm = 0; // 1;
	mac_config.dm = 1;
	mac_config.fes = 1;
	mac_config.je = 1;
	mac_config.jd = 1;
	mac_config.wd = 1;
	mac_config.ipc = 1;
	mac_config.sarc = 0x02;
	volatile uint32_t* mac_config_reg = (uint32_t*) (enet0_addr + mac_config_reg_addr);
	*mac_config_reg = *((uint32_t*) &mac_config);

	delay_us(100); // Delay consecutive write to the same register, as the sysclock and the enet0 clock are on different domains

	mac_config.re = 1;
	mac_config.te = 1;
	*mac_config_reg = *((uint32_t*) &mac_config);

	return ENET_OK;
}

int init_enet_clock(void) {
	enet_rmii_clk_select(0x00); // Do not use any internally managed/generated clock, for the ENET0_TX_CLK
	set_enet_rmii_clk_div(0x00);
	enet_ptp_ref_clk_select(0x04);
	set_enet_ptp_ref_clk_div(0x00);
	return ENET_OK;
}

int enet_set_smi(uint32_t clk_freq) {
    const uint32_t clk_mhz = clk_freq / 1000000;
    if ((clk_mhz < 20) || (clk_mhz > 800)) return -ENET_INIT_FAIL;

    uint32_t cr  = 0;
    if (clk_mhz < 35)       cr = 2;
    else if (clk_mhz < 60)  cr = 3;
    else if (clk_mhz < 100) cr = 0;
    else if (clk_mhz < 150) cr = 1;
    else if (clk_mhz < 250) cr = 4;
    else if (clk_mhz < 300) cr = 5;
    else if (clk_mhz < 500) cr = 6;
    else if (clk_mhz < 800) cr = 7;

    volatile uint32_t* mac_mdio_address_reg = (uint32_t*) (enet0_addr + mac_mdio_address_reg_addr);
    mac_mdio_address_t mac_mdio_address = {0};
    mac_mdio_address.cr = cr;
    *mac_mdio_address_reg = *((uint32_t*) &mac_mdio_address);

    return ENET_OK;
}

uint32_t enet_phy_read(const uint32_t phy_addr, const uint32_t phy_reg) {
	volatile uint32_t* mac_mdio_address_reg = (uint32_t*) (enet0_addr + mac_mdio_address_reg_addr);
	mac_mdio_address_t mac_mdio_address = *(mac_mdio_address_t*)mac_mdio_address_reg;
	mac_mdio_address.goc_0 = 1;
	mac_mdio_address.goc_1 = 1;
	mac_mdio_address.pa_low = phy_addr & 0x07;
	mac_mdio_address.pa_high = (phy_addr >> 3) & 0x03;
	mac_mdio_address.rda = phy_reg;
	*mac_mdio_address_reg = *((uint32_t*) &mac_mdio_address);

	mac_mdio_address.gb = 1;
	*mac_mdio_address_reg = *((uint32_t*) &mac_mdio_address);

	while (*mac_mdio_address_reg & 0x01) delay_us(1);

    const unsigned int mac_mdio_data_reg_addr = 0x204;
    volatile uint32_t* mac_mdio_data = (uint32_t*) (enet0_addr + mac_mdio_data_reg_addr);
    return (*mac_mdio_data & 0xFFFF);
}

__attribute__((constructor(104))) int init_enet(void) {
	set_ahb_clk_ctrl(0, (1U << PORT0) | (1U << PORT1) | (1U << PORT2) | (1U << PORT3) | (1U << PORT4) | (1 << INPUT_MUX));
	int err = 0;

	// NOTE: P1_4,5,6,7 - P1_13,14,15 and P1_20,21 are the essential ones, the other could theoretically be skipped but would require
	const uint8_t enet_pins[] = { 4, 5, 6, 7, 13, 14, 15, 20, 21 };
	//const uint8_t enet_pins[] = { 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
	for (unsigned int i = 0; i < sizeof(enet_pins); ++i) {
		err = update_pin_function(1, enet_pins[i], (9U << MUX), MUX_BITMASK);
		if (err) return -ENET_INIT_FAIL;
		update_pin_function(1, enet_pins[i], (1U << IBE), IBE_BITMASK);
	}

	err = init_enet_clock();
	if (err) return -ENET_INIT_FAIL;

	set_ahb_clk_ctrl(2, (1U << ENET));
	set_enet_phy_interface(ENET_RMII_PHY);
	set_peripheral_reset_ctrl(2, (1U << ENET_RST));
	clear_peripheral_reset_ctrl(2, (1U << ENET_RST));

	set_enet_sbd_flow_ctrl(0x03);

	err = init_enet_dma();
	if (err) return -ENET_INIT_FAIL;

	err = init_enet_mtl_regs();
	if (err) return -ENET_INIT_FAIL;

	err = init_enet_mac();
	if (err) return -ENET_INIT_FAIL;

	nvic_enable_irq(IRQ_ENET0);
	nvic_enable_irq(IRQ_ENET0_PM);
	nvic_enable_irq(IRQ_ENET0_LPI);

	set_enet_sbd_flow_ctrl(0x00);

	return ENET_OK;
}

void get_enet_status(uint32_t* status) {
	volatile uint32_t* mac_int_status = (uint32_t*) (enet0_addr + mac_int_status_reg_addr);
	status[0] = *mac_int_status;

	volatile uint32_t* mtl_int_status = (uint32_t*) (enet0_addr + mtl_int_status_reg_addr);
	status[1] = *mtl_int_status;

	volatile uint32_t* dma_int_status = (uint32_t*) (enet0_addr + dma_int_status_reg_addr);
	status[2] = *dma_int_status;

	volatile uint32_t* dma_status_ch0 = (uint32_t*) (enet0_addr + dma_status_ch0_reg_addr);
	status[3] = *dma_status_ch0;

	volatile uint32_t* dma_status_ch1 = (uint32_t*) (enet0_addr + dma_status_ch1_reg_addr);
	status[4] = *dma_status_ch1;

	volatile uint32_t* mac_debug = (uint32_t*) (enet0_addr + mac_debug_reg_addr);
	status[5] = *mac_debug;

	const unsigned int dma_ch0_cur_app_txdesc_addr = 0x1144;
	volatile uint32_t* dma_ch0_cur_app_txdesc = (uint32_t*) (enet0_addr + dma_ch0_cur_app_txdesc_addr);
	status[6] = *dma_ch0_cur_app_txdesc;

	const unsigned int dma_ch0_cur_app_rxdesc_addr = 0x114C;
	volatile uint32_t* dma_ch0_cur_app_rxdesc = (uint32_t*) (enet0_addr + dma_ch0_cur_app_rxdesc_addr);
	status[7] = *dma_ch0_cur_app_rxdesc;

	const unsigned int dma_ch0_cur_app_txbuf_addr = 0x1154;
	volatile uint32_t* dma_ch0_cur_app_txbuf = (uint32_t*) (enet0_addr + dma_ch0_cur_app_txbuf_addr);
	status[8] = *dma_ch0_cur_app_txbuf;

	const unsigned int dma_ch0_cur_app_rxbuf_addr = 0x115C;
	volatile uint32_t* dma_ch0_cur_app_rxbuf = (uint32_t*) (enet0_addr + dma_ch0_cur_app_rxbuf_addr);
	status[9] = *dma_ch0_cur_app_rxbuf;

	const unsigned int dma_ch1_cur_app_txdesc_addr = 0x11C4;
	volatile uint32_t* dma_ch1_cur_app_txdesc = (uint32_t*) (enet0_addr + dma_ch1_cur_app_txdesc_addr);
	status[10] = *dma_ch1_cur_app_txdesc;

	const unsigned int dma_ch1_cur_app_rxdesc_addr = 0x11CC;
	volatile uint32_t* dma_ch1_cur_app_rxdesc = (uint32_t*) (enet0_addr + dma_ch1_cur_app_rxdesc_addr);
	status[11] = *dma_ch1_cur_app_rxdesc;

	const unsigned int dma_ch1_cur_app_txbuf_addr = 0x11D4;
	volatile uint32_t* dma_ch1_cur_app_txbuf = (uint32_t*) (enet0_addr + dma_ch1_cur_app_txbuf_addr);
	status[12] = *dma_ch1_cur_app_txbuf;

	const unsigned int dma_ch1_cur_app_rxbuf_addr = 0x11DC;
	volatile uint32_t* dma_ch1_cur_app_rxbuf = (uint32_t*) (enet0_addr + dma_ch1_cur_app_rxbuf_addr);
	status[13] = *dma_ch1_cur_app_rxbuf;

	return;
}

const uint32_t header[512] = {0};
const uint32_t payload[512] = {0};

void test_enet_tx(void) {
	ch0_tx_desc_rb[0].buf1_ap = (uint32_t) header;
	ch0_tx_desc_rb[0].buf2_ap = (uint32_t) payload;
	ch0_tx_desc_rb[0].b1l = 512;
	ch0_tx_desc_rb[0].b2l = 512;
	ch0_tx_desc_rb[0].ioc = 1;
	ch0_tx_desc_rb[0].fl = 1024;
	ch0_tx_desc_rb[0].cic = 0;
	ch0_tx_desc_rb[0].saic_low = 0;
	ch0_tx_desc_rb[0].saic_high = 0;
	ch0_tx_desc_rb[0].cpc = 0x02;

	ch0_tx_desc_rb[0].fd = 1;
	ch0_tx_desc_rb[0].ld = 1;
	ch0_tx_desc_rb[0].own = 1;

	volatile uint32_t* dma_ch0_tx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch0_tx_tail_ptr_reg_addr);
	*dma_ch0_tx_tail_ptr = ((uint32_t) (ch0_tx_desc_rb + 1)) & 0xFFFFFFFC;

	return;
}

static uint32_t status[15] = {0};

void parse_rx_packet(void) {
	return;
}

void parse_tx_result(void) {
	return;
}

void dma_ch_int_handler(volatile dma_ch_int_status_t* dma_ch_status) {
	if (dma_ch_status -> nis && !dma_ch_status -> ais && !dma_ch_status -> reb && !dma_ch_status -> teb) {
		if (dma_ch_status -> ri) parse_rx_packet();
		if (dma_ch_status -> ti) parse_tx_result();
	}

	// Self write, effectively writing set bits in order to auto-clear them.
	*dma_ch_status = *dma_ch_status;

	return;
}

void dma_int_handler(volatile dma_int_status_t* dma_int_status) {
	volatile dma_ch_int_status_t* dma_status_ch0 = (dma_ch_int_status_t*) (enet0_addr + dma_status_ch0_reg_addr);
	volatile dma_ch_int_status_t* dma_status_ch1 = (dma_ch_int_status_t*) (enet0_addr + dma_status_ch1_reg_addr);

	if      (dma_int_status -> dc0_is) dma_ch_int_handler(dma_status_ch0);
	else if (dma_int_status -> dc1_is) dma_ch_int_handler(dma_status_ch1);

	return;
}

void mtl_int_handler(void) {
	return;
}

void mac_int_handler(void) {
	return;
}

__attribute__((section(".after_vectors.enet_handler"), naked, noreturn)) void enet_handler(void) {
	volatile dma_int_status_t* dma_int_status = (dma_int_status_t*) (enet0_addr + dma_int_status_reg_addr);
	if (dma_int_status -> dc0_is || dma_int_status -> dc1_is) dma_int_handler(dma_int_status);
	if (dma_int_status -> mtl_is) mtl_int_handler();
	if (dma_int_status -> mac_is) mac_int_handler();

	// TODO: Clear the interrupt status flags, once they have been handled
	__asm__ ("bkpt");

	__asm__ volatile("bx lr");
}

__attribute__((section(".after_vectors.enet_pm_handler"), naked, noreturn)) void enet_pm_handler(void) {
	__asm__ volatile("bkpt");
	__asm__ volatile("bx lr");
}

__attribute__((section(".after_vectors.enet_lpi_handler"), naked, noreturn)) void enet_lpi_handler(void) {
	__asm__ volatile("bkpt");
	__asm__ volatile("bx lr");
}

#endif //_ENET_H_
