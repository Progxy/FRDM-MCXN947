#ifndef _LTR_H_
#define _LTR_H_

#include <stdint.h>
#include <i3c.h>
#include <utils.h>

#define MAX_ATTEMPTS  10

typedef enum {
	MAIN_CTRL         = 0x00,
	ALS_UVS_MEAS_RATE = 0x04,
  	ALS_UVS_GAIN      = 0x05,
	MAIN_STATUS       = 0x07,
    ALS_DATA          = 0x0D,
	UVS_DATA          = 0x10,
    UVS_COMP_DATA     = 0x13,
    COMP_DATA         = 0x16
} LTRRegisters;

typedef enum {
	LTR_ADDRESS           = 0x53,
	ALS_UVS_DATA_STATUS   = 0x08,
    BASE_UV_COUNTS        = 1810,
	UV_SENSITIVITY        = 2300,
	LTR_INTERRUPT_PIN     = 19
} LTRConstants;

typedef enum {
	MAX_RATE = 0,
    MS_25    = 0,
	MS_50    = 1,
	MS_100   = 2,
	MS_200   = 3,
	MS_500   = 4,
	MS_1000  = 5,
	MS_2000  = 6,
	MIN_RATE = 7 // min rate is still 2000ms
} LTRMeasurementRate;

typedef enum {
	MAX_RES = 0,
    BIT_20  = 0,
  	BIT_19  = 1,
	BIT_18  = 2,
	BIT_17  = 3,
	BIT_16  = 4,
	BIT_13  = 5,
	MIN_RES = 5
} LTRResolutions;

typedef enum {
  	MIN_GAIN = 0,
  	GAIN_1   = 0,
    GAIN_2   = 1,
    GAIN_3   = 2,
  	GAIN_4   = 3,
    GAIN_5   = 4,
  	MAX_GAIN = 4,
} LTRGainLevel;

typedef enum {
  ALS_MODE = 0,
  UVS_MODE = 1,
  OFF      = 0,
  ON       = 1
} LTRMode;

typedef struct {
  uint8_t measurement_rate: 3;
  uint8_t reserved: 1;
  uint8_t resolution: 3;
  uint8_t rsv: 1;
} __attribute__((packed)) als_uvs_meas_rate_t;

static_assert(sizeof(als_uvs_meas_rate_t) == 1, "Size must be 1 byte");

static const uint16_t measurement_rate_ms[8] = { 25, 50, 100, 200, 500, 1000, 2000, 2000 };
static const uint8_t res_bit_masks[6]  = { 0x0F, 0x07, 0x03, 0x01, 0xFF, 0x1F };
static uint8_t main_status                      = 0x20;
static uint8_t main_ctrl                        = 0x00;
static uint8_t gain                             = 0x01;
static als_uvs_meas_rate_t als_uvs_meas_rate = {0};

static uint8_t ltr_read_register(uint8_t reg, bool terminate) {
	uint8_t status = send_i2c(LTR_ADDRESS, &reg, 1, FALSE);
	if (status) __asm__("bkpt");

	uint8_t data = 0;
	status = recv_i2c(LTR_ADDRESS, &data, 1, terminate);
	if (status) __asm__("bkpt");

	return data;
}

static void ltr_write_register(uint8_t reg, uint8_t data) {
	uint8_t d[2] = {reg, data};
	uint8_t status = send_i2c(LTR_ADDRESS, d, 2, TRUE);
	if (status) __asm__("bkpt");
	return;
}

static uint32_t ltr_read_data_reg(uint8_t reg_addr) {
	uint32_t data = 0;
	uint8_t res_bit_mask = res_bit_masks[als_uvs_meas_rate.resolution];

	if (als_uvs_meas_rate.resolution < 4) {
		data = ltr_read_register(reg_addr + 2, FALSE) & res_bit_mask;
		res_bit_mask = 0xFF;
	}

	data = (data << 8) | (ltr_read_register(reg_addr + 1, FALSE) & res_bit_mask);
	data = (data << 8) | ltr_read_register(reg_addr, TRUE);

	return data;
}

static bool ltr_is_data_available(void) {
	main_status = ltr_read_register(MAIN_STATUS, TRUE);
	return main_status & ALS_UVS_DATA_STATUS;
}

uint32_t to_uvi(uint32_t val) {
	const float window_factor = 1.0f;
	const float time_factor = measurement_rate_ms[als_uvs_meas_rate.measurement_rate] / 400.0f;
	return (((float) val) / (UV_SENSITIVITY * time_factor)) * window_factor;
}

static void ltr_set_mode(LTRMode ltr_mode) {
	main_ctrl = (main_ctrl & (~(ltr_mode << 3))) | (ltr_mode << 3);
	ltr_write_register(MAIN_CTRL, main_ctrl);
	return;
}

static void ltr_set_gain(LTRGainLevel gain_lvl) {
	gain = gain_lvl;
	ltr_write_register(ALS_UVS_GAIN, gain);
	return;
}

static void ltr_set_resolution(LTRResolutions res) {
	als_uvs_meas_rate.resolution = res;
	ltr_write_register(ALS_UVS_MEAS_RATE, *((uint8_t*) &(als_uvs_meas_rate)));
	return;
}

static void ltr_set_measurement_rate(LTRMeasurementRate meas) {
	als_uvs_meas_rate.measurement_rate = meas;
	ltr_write_register(ALS_UVS_MEAS_RATE, *((uint8_t*) &(als_uvs_meas_rate)));
	return;
}

static void ltr_toggle_sensor(LTRMode status) {
	main_ctrl = (main_ctrl & (~(status << 1))) | (status << 1);
	ltr_write_register(MAIN_CTRL, main_ctrl);
	return;
}

uint32_t ltr_sample_uvi(void) {
	for (uint8_t i = 0; !(ltr_is_data_available()); ++i) {
		if (i == MAX_ATTEMPTS) return 0;
		delay_ms(500);
	}
	uint32_t uvi = to_uvi(ltr_read_data_reg(UVS_DATA));
	return uvi;
}

void init_ltr(void) {
	main_ctrl = ltr_read_register(MAIN_CTRL, TRUE);
	uint8_t als_uvs_meas_rate_ = ltr_read_register(ALS_UVS_MEAS_RATE, TRUE);
	als_uvs_meas_rate = *((als_uvs_meas_rate_t*) &als_uvs_meas_rate_);
	main_status = ltr_read_register(MAIN_STATUS, TRUE);
	gain = ltr_read_register(ALS_UVS_GAIN, TRUE) & 0x7;

	ltr_set_gain(MAX_GAIN);
	ltr_set_resolution(MAX_RES);
	ltr_set_measurement_rate(MIN_RATE);
	ltr_set_mode(UVS_MODE);
	ltr_toggle_sensor(ON);

	return;
}

#endif // _LTR_H_
