#ifndef _USB_PHY_H_
#define _USB_PHY_H_

typedef struct {
	uint8_t en_otg_id_chg_irq: 1;
	uint8_t en_host_discon_detect: 1;
	uint8_t en_irq_host_discon: 1;
	uint8_t host_discon_detect_irq: 1;
	uint8_t en_dev_plugin_detect: 1;
	uint8_t dev_plugin_polarity: 1;
	uint8_t otg_id_chg_irq: 1;
	uint8_t en_otg_id_detect: 1;

	uint8_t resume_irq_sticky: 1;
	uint8_t en_irq_resume_detect: 1;
	uint8_t resume_irq: 1;
	uint8_t en_irq_dev_plugin: 1;
	uint8_t dev_plugin_irq: 1;
	uint8_t data_on_lra_dc: 1;
	uint8_t en_utmi_level2: 1;
	uint8_t en_utmi_level3: 1;

	uint8_t en_irq_wakeup: 1;
	uint8_t wakeup_irq: 1;
	uint8_t autoresume_en: 1;
	uint8_t en_autoclr_clkgate: 1;
	uint8_t en_autoclr_phy_pwd: 1;
	const uint8_t rsv: 3;

	const uint8_t rsv1: 3;
	uint8_t otg_id_value: 1;
	const uint8_t rsv2: 1;
	uint8_t utmi_suspendm: 1;
	uint8_t clkgate: 1;
	uint8_t sftrst: 1;
} __attribute__((packed)) usb_phy_ctrl_t;

typedef struct {
	uint8_t rsv: 5;
	uint8_t misc2_control0: 1;
	uint8_t pll_en_usb_clks: 1;
	uint8_t rsv1: 1;

	uint8_t rsv2: 4;
	uint8_t pll_power: 1;
	uint8_t pll_enable: 1;
	uint8_t rsv3: 2;

	uint8_t pll_bypass: 1;
	uint8_t rsv4: 2;
	uint8_t refbias_pwd_sel: 1;
	uint8_t refbias_pwd: 1;
	uint8_t pll_reg_enable: 1;
	uint8_t pll_div_sel_low: 2;

	uint8_t pll_div_sel_high: 1;
	uint8_t rsv5: 5;
	uint8_t pll_prediv: 1;
	uint8_t pll_lock: 1;
} __attribute__((packed)) usb_phy_pll_sic_t;

typedef struct {
	const uint8_t rsv: 1;
	uint8_t lvi_en: 1;
	uint8_t pfd_clk_sel: 2;
	const uint8_t rsv1: 4;
	const uint8_t rsv2: 2;
	uint8_t dev_pulldown: 1;
	const uint32_t rsv3: 21;
} __attribute__((packed)) usb_phy_anactrl_t;

static_assert(sizeof(usb_phy_ctrl_t) == 4,    "Size must be 4 bytes");
static_assert(sizeof(usb_phy_pll_sic_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(usb_phy_anactrl_t) == 4, "Size must be 4 bytes");

const unsigned int usb_phy_addr = 0x4010A000;
const unsigned int usb_phy_pwd_offset = 0x00;
const unsigned int usb_phy_ctrl_set_offset = 0x34;
const unsigned int usb_phy_ctrl_clr_offset = 0x38;
const unsigned int usb_phy_pll_sic_offset = 0xA0;
const unsigned int usb_phy_pll_sic_set_offset = 0xA4;
const unsigned int usb_phy_pll_sic_clr_offset = 0xA8;
const unsigned int usb_phy_anactrl_offset = 0x100;
const unsigned int usb_phy_anactrl_set_offset = 0x104;

int enable_usb_phy(void) {
	volatile usb_phy_ctrl_t* usb_phy_ctrl_clr = (usb_phy_ctrl_t*) (usb_phy_addr + usb_phy_ctrl_clr_offset);
	volatile usb_phy_ctrl_t* usb_phy_ctrl_set = (usb_phy_ctrl_t*) (usb_phy_addr + usb_phy_ctrl_set_offset);
	volatile uint32_t* usb_phy_pwd = (uint32_t*) (usb_phy_addr + usb_phy_pwd_offset);
	volatile usb_phy_pll_sic_t* usb_phy_pll_sic = (usb_phy_pll_sic_t*) (usb_phy_addr + usb_phy_pll_sic_offset);
	volatile usb_phy_anactrl_t* usb_phy_anactrl_set = (usb_phy_anactrl_t*) (usb_phy_addr + usb_phy_anactrl_set_offset);

	// NOTE: to the FRDM-MCXN947 external clock reference pin is connected a 24MHz XOSC
	int err = update_sosc_clk_freq(SOSC_20_30MHZ);
	if (err) return -1;

	clock_ctrl_set(SYSCON_CLKIN_ENA_FM_USBH_LPT_MASK | SYSCON_CLKIN_ENA_MASK);
	set_enable_clk_ctrl(2, (1U << 16) | (1U << 17));

	usb_phy_ctrl_clr -> sftrst = 1;
	usb_phy_ctrl_clr -> clkgate = 1;
	*usb_phy_pwd = 0x00;
	usb_phy_ctrl_set -> en_utmi_level2 = 1;
	usb_phy_ctrl_set -> en_utmi_level3 = 1;

	usb_phy_ctrl_set -> en_autoclr_phy_pwd = 1;
	usb_phy_ctrl_set -> en_autoclr_clkgate = 1;

	usb_phy_pll_sic -> pll_div_sel_low = 0x02;
	usb_phy_pll_sic -> pll_div_sel_high = 0x00;
	usb_phy_pll_sic -> pll_reg_enable = 1;
	delay_us(20);

	usb_phy_pll_sic -> pll_bypass = 0;
	usb_phy_pll_sic -> pll_power = 1;
	usb_phy_anactrl_set -> lvi_en = 1;

	while (!usb_phy_pll_sic -> pll_lock) delay_us(1);
	usb_phy_pll_sic -> pll_en_usb_clks = 1;

	return 0;
}

#endif //_USB_PHY_H_
