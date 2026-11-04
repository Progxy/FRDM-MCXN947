#ifndef _ENET_H_
#define _ENET_H_

#include <utils.h>
#include <syscon.h>
#include <alloc.h>

typedef enum { ENET_OK = 0, ENET_INIT_FAIL, } ENETStatus;
typedef enum { ENET_MII_PHY = 0, ENET_RMII_PHY = 1 } ENETInterfaceType;
typedef enum { ENET_PBL1 = 1, ENET_PBL2 = 2, ENET_PBL4 = 4, ENET_PBL8 = 8, ENET_PBL16 = 16, ENET_PBL32 = 32 } ENETBurstLenghts;

const uint16_t mac_address_high = 0x4E03;
const uint32_t mac_address_low  = 0x304AD7EE;

const unsigned int enet0_addr = 0x40100000;
const unsigned int dma_mode_reg_addr = 0x1000;
const unsigned int dma_sysbus_mode_reg_addr = 0x1004;
const unsigned int dma_ch0_rx_ctrl2_reg_addr = 0x1130;
const unsigned int dma_ch1_rx_ctrl2_reg_addr = 0x11B0;
const unsigned int dma_ch0_tx_rbl_reg_addr = 0x112C;
const unsigned int dma_ch1_tx_rbl_reg_addr = 0x11AC;
const unsigned int dma_ch0_tx_addr_reg_addr = 0x1114;
const unsigned int dma_ch0_rx_addr_reg_addr = 0x111C;
const unsigned int dma_ch1_tx_addr_reg_addr = 0x119C;
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
const unsigned int mac_config_reg_addr = 0x00;

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

int init_enet_dma(void) {
	volatile uint32_t* dma_mode_reg = (uint32_t*) (enet0_addr + dma_mode_reg_addr);
	dma_mode_t dma_mode = *(dma_mode_t*)dma_mode_reg;
	dma_mode.swr = 1;
	*dma_mode_reg = *((uint32_t*) &dma_mode);

	while (*dma_mode_reg & 0x01) delay_us(1);

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
	for (unsigned int i = 0; i < DMA_DESC_RB_SIZE - 1; ++i) {
		ch0_rx_desc_rb[i].own = 1;
		ch1_rx_desc_rb[i].own = 1;
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
	*dma_ch0_tx_addr = ((uint32_t) ch0_tx_desc_rb) & 0xFFF4;
	*dma_ch1_tx_addr = ((uint32_t) ch1_tx_desc_rb) & 0xFFF4;

	volatile uint32_t* dma_ch0_rx_addr = (uint32_t*) (enet0_addr + dma_ch0_rx_addr_reg_addr);
	volatile uint32_t* dma_ch1_rx_addr = (uint32_t*) (enet0_addr + dma_ch1_rx_addr_reg_addr);
	*dma_ch0_rx_addr = ((uint32_t) ch0_rx_desc_rb) & 0xFFF4;
	*dma_ch1_rx_addr = ((uint32_t) ch1_rx_desc_rb) & 0xFFF4;

	volatile uint32_t* dma_ch0_tx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch0_tx_tail_ptr_reg_addr);
	volatile uint32_t* dma_ch1_tx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch1_tx_tail_ptr_reg_addr);
	*dma_ch0_tx_tail_ptr = ((uint32_t) ch0_tx_desc_rb) & 0xFFF4;
	*dma_ch1_tx_tail_ptr = ((uint32_t) ch1_tx_desc_rb) & 0xFFF4;

	volatile uint32_t* dma_ch0_rx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch0_rx_tail_ptr_reg_addr);
	volatile uint32_t* dma_ch1_rx_tail_ptr = (uint32_t*) (enet0_addr + dma_ch1_rx_tail_ptr_reg_addr);
	*dma_ch0_rx_tail_ptr = ((uint32_t) ch0_rx_desc_rb) & 0xFFF4;
	*dma_ch1_rx_tail_ptr = ((uint32_t) ch1_rx_desc_rb) & 0xFFF4;

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
	//dma_ch_inte.tie = 1;
	dma_ch_inte.txse = 1;
	dma_ch_inte.tbue = 1;
	//dma_ch_inte.rie = 1;
	dma_ch_inte.rbue = 1;
	dma_ch_inte.rse = 1;
	dma_ch_inte.rwte = 1;
	dma_ch_inte.fbee = 1;
	dma_ch_inte.cdee = 1;
	dma_ch_inte.aie = 1;
	//dma_ch_inte.nie = 1;

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

__attribute__((constructor(104))) int init_enet(void) {
	set_ahb_clk_ctrl(0, (1U << PORT0) | (1U << PORT1) | (1U << PORT2) | (1U << PORT3) | (1U << PORT4));
	set_ahb_clk_ctrl(2, (1U << ENET));
	set_peripheral_reset_ctrl(2, (1U << ENET_RST));
	set_enet_phy_interface(ENET_MII_PHY);

	// TODO: Check P1_2-P1_21 ALT pin assignment 9
	int err = 0;
	for (unsigned int i = 2; i < 22; ++i) {
		err = set_pin_function(1, i, (9U << 8));
		if (err) return -ENET_INIT_FAIL;
	}

	// TODO: Set the sideband flow control for each channel.

	err = init_enet_dma();
	if (err) return -ENET_INIT_FAIL;

	err = init_enet_mtl_regs();
	if (err) return -ENET_INIT_FAIL;

	err = init_enet_mac();
	if (err) return -ENET_INIT_FAIL;

	return ENET_OK;
}

#endif //_ENET_H_
