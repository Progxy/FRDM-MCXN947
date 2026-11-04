#ifndef _SYSCON_H_
#define _SYSCON_H_

#include <stdint.h>

const unsigned int syscon_addr = 0x40000000;
const unsigned int syscon_periph_reset_ctrl_set = 0x120;
const unsigned int clk_ahb_ctrl_set_offsets[] = { 0x220, 0x224, 0x228, 0x22C };
const unsigned int clk_ahb_ctrl_clear_offsets[] = { 0x240, 0x244, 0x248, 0x24C };
const unsigned int syscon_fc_clk_sel_base = 0x2B0;
const unsigned int syscon_i3c0_clock_sel = 0x530;
const unsigned int syscon_i3c0_clock_div = 0x540;
const unsigned int syscon_enet_phy_intf_sel = 0x5C0;
const unsigned int syscon_clock_ctrl_offset = 0xA18;
const unsigned int syscon_i3c1_clock_sel = 0xB30;
const unsigned int syscon_i3c1_clock_div = 0xB40;

typedef enum { ROM = 1, RAMB, RAMC, RAMD, RAME, RAMF, RAMG, RAMH, FMU, FMC, FLEXSPI, MUX, PORT0, PORT1, PORT2, PORT3, PORT4, GPIO0 = 19, GPIO1, GPIO2, GPIO3, GPIO4, PINT = 25, DMA0, CRC, WWDT0, WWDT1, MAILBOX = 31 } AHBControlClocks0;
typedef enum { MRT = 0, OSTIMER, ADC0, ADC1, DAC0, RTC, EVSIM0 = 8, EVSIM1, UTICK, FC0, FC1, FC2, FC3, FC4, FC5, FC6, FC7, FC8, FC9, MICFIL, TIMER2, USB0_FS_DCD = 24, USB0_FS, TIMER0, TIMER1, SMART_DMA = 31 } AHBControlClocks1;
typedef enum { DMA1 = 1, ENET, USDHC, FLEXIO, SAI0, SAI1, FREQME = 8, FLEXCAN0 = 14, FLEXCAN1, USB_HS, USB_HS_PHY, PQ = 19, PLU_LUT, TIMER3, TIMER4, SCG = 26 } AHBControlClocks2;
typedef enum { I3C_0 = 0, I3C_1, SINC, COOL_FLUX, QDC0, QDC1, PWM0, PWM1, EVTG, DAC1 = 11, DAC2, OPAMP0, OPAMP1, OPAMP2, CMP2 = 18, VREF, COOL_FLUX_APB, NPU, TSI, EWM, EIM, ERM, INTM, SEMA42 } AHBControlClocks3;

typedef enum { DMA1_RST = 1, ENET_RST, USDHC_RST, FLEXIO_RST, SAI0_RST, SAI1_RST, FREQME_RST = 8, FLEXCAN0_RST = 14, FLEXCAN1_RST, USB_HS_RST, USB_HS_PHY_RST, PQ_RST = 19, PLU_LUT_RST, TIMER3_RST, TIMER4_RST } PeripheralResetCtrl2;

void set_ahb_clk_ctrl(unsigned int clk_ctrl_idx, const uint32_t value) {
  volatile uint32_t* const addr = (uint32_t*) (syscon_addr + clk_ahb_ctrl_set_offsets[clk_ctrl_idx]);
  *addr = value;
  return;
}

void clear_ahb_clk_ctrl(unsigned int clk_ctrl_idx, const uint32_t value) {
  volatile uint32_t* const addr = (uint32_t*) (syscon_addr + clk_ahb_ctrl_clear_offsets[clk_ctrl_idx]);
  *addr = value;
  return;
}

#define SYSCON_CLOCK_CTRL_MASK_A 0x000002FF
#define SYSCON_CLOCK_CTRL_MASK_B 0x00000081
void clock_ctrl_set(uint32_t bitmask) {
	volatile uint32_t* syscon_clock_ctrl = (uint32_t*) (syscon_addr + syscon_clock_ctrl_offset);
	*syscon_clock_ctrl = (*syscon_clock_ctrl | bitmask | SYSCON_CLOCK_CTRL_MASK_B) & SYSCON_CLOCK_CTRL_MASK_A;
	return;
}

void clock_ctrl_clr(uint32_t bitmask) {
	volatile uint32_t* syscon_clock_ctrl = (uint32_t*) (syscon_addr + syscon_clock_ctrl_offset);
	*syscon_clock_ctrl = ((*syscon_clock_ctrl & ~bitmask) | SYSCON_CLOCK_CTRL_MASK_B) & SYSCON_CLOCK_CTRL_MASK_A;
	return;
}

void set_i3c0_clock(uint8_t sel, uint8_t div) {
	volatile uint32_t* i3c0_clock_sel = (uint32_t*) (syscon_addr + syscon_i3c0_clock_sel);
	*i3c0_clock_sel = sel & 0x07;
	volatile uint32_t* i3c0_clock_div = (uint32_t*) (syscon_addr + syscon_i3c0_clock_div);
	*i3c0_clock_div = div & 0xFF;
	return;
}

void set_i3c1_clock(uint8_t sel, uint8_t div) {
	volatile uint32_t* i3c1_clock_sel = (uint32_t*) (syscon_addr + syscon_i3c1_clock_sel);
	*i3c1_clock_sel = sel & 0x07;
	volatile uint32_t* i3c1_clock_div = (uint32_t*) (syscon_addr + syscon_i3c1_clock_div);
	*i3c1_clock_div = div & 0xFF;
	return;
}

void enable_flexcomm_clock(uint8_t id, uint8_t sel) {
	volatile uint32_t* fc_clk_sel = (uint32_t*) (syscon_addr + syscon_fc_clk_sel_base + id * 4);
	*fc_clk_sel = sel & 0x07;
	return;
}

void set_peripheral_reset_ctrl(uint8_t idx, uint32_t val) {
	if (idx > 3) return;
	volatile uint32_t* preset_ctrl_set = (uint32_t*) (syscon_addr + syscon_periph_reset_ctrl_set + 4 * idx);
	*preset_ctrl_set = val;
	return;
}

void set_enet_phy_interface(uint8_t interface) {
	volatile uint32_t* enet_phy_intf_sel = (uint32_t*) (syscon_addr + syscon_enet_phy_intf_sel);
	*enet_phy_intf_sel |= (interface & 0x01) << 2;
	return;
}

#endif //_SYSCON_H_
