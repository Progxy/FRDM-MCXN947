#ifndef _BME_H_
#define _BME_H_

#include <stdint.h>
#include <fpu.h>
#include <i3c.h>
#include <utils.h>

typedef enum {
	BME_ADDRESS = 0x76,

	// Calibration Data Registers
	BME_TP_CD = 0x88,
	BME_H1_CD = 0xA1,
	BME_H2_CD = 0xE1,
	BME_H4_CD = 0xE4,
	BME_H6_CD = 0xE7,

	// Misc Registers
	BME_ID        = 0xD0,
	BME_CTRL_HUM  = 0xF2,
	BME_STATUS    = 0xF3,
	BME_CTRL_MEAS = 0xF4,
	BME_DATA      = 0xF7
} BMERegisters;

typedef enum {
	SLEEP_MODE  = 0x00,
	FORCED_MODE = 0x01,
	NORMAL_MODE = 0x03
} BMEMode;

typedef struct {
	uint8_t ctrl_hum;
	union {
		struct {
			uint8_t mode:   2;
			uint8_t osrs_p: 3;
			uint8_t osrs_t: 3;
		} __attribute__((packed));
		uint8_t ctrl_meas;
	};
} __attribute__((packed)) bme_ctrl_t;

typedef struct {
	struct {
	  uint16_t t1;
	  int16_t  t2;
	  int16_t  t3;
	} __attribute__((packed)) bme_tcd;

	struct {
	  uint16_t p1;
	  int16_t  p2;
	  int16_t  p3;
	  int16_t  p4;
	  int16_t  p5;
	  int16_t  p6;
	  int16_t  p7;
	  int16_t  p8;
	  int16_t  p9;
	} __attribute__((packed)) bme_pcd;

	struct {
	  uint8_t  h1;
	  int16_t  h2;
	  uint8_t  h3;
	  int16_t  h4;
	  int16_t  h5;
   	  int8_t   h6;
	} __attribute__((packed)) bme_hcd;
} __attribute__((packed)) bme_cd_t;

typedef struct {
  // Pressure Reading (16 - 20 bit)
  uint8_t p_msb;
  uint8_t p_lsb;
  uint8_t p_xlsb;

  // Temperature Reading (16 - 20 bit)
  uint8_t t_msb;
  uint8_t t_lsb;
  uint8_t t_xlsb;

  // Humidity Reading (16 bit)
  uint8_t h_msb;
  uint8_t h_lsb;
} __attribute__((packed)) bme_raw_data_t;

typedef struct {
	int32_t  temperature;
	uint32_t pressure;
	uint32_t humidity;
} bme_data_t;

static bme_cd_t bme_cd = {0};
static bme_ctrl_t bme_ctrl = {0};
static int32_t t_fine = 0;

static void bme_read_registers(const uint8_t reg, uint8_t* res, const int count) {
	int status = __send_i2c(I3C1, BME_ADDRESS, &reg, 1, FALSE);
	if (status) __asm__("bkpt");
	status = __recv_i2c(I3C1, BME_ADDRESS, &res, count, TRUE);
	if (status) __asm__("bkpt");
	return;
}

static void bme_write_register(const uint8_t reg, const uint8_t data) {
	const uint8_t buf[2] = { reg, data };
	int status = __send_i2c(I3C1, BME_ADDRESS, &buf, 2, TRUE);
	if (status) __asm__("bkpt");
	return;
}

static void write_bme_ctrl(void) {
	bme_write_register(BME_CTRL_HUM, bme_ctrl.ctrl_hum);
	bme_write_register(BME_CTRL_MEAS, *((uint8_t*) &(bme_ctrl.ctrl_meas)));
	return;
}

static void bme_get_calibration_data(void) {
	bme_read_registers(BME_TP_CD, (uint8_t*) &bme_cd, 24);
	bme_read_registers(BME_H1_CD, ((uint8_t*) &(bme_cd.bme_hcd.h1)), 1);
	bme_read_registers(BME_H2_CD, ((uint8_t*) &(bme_cd.bme_hcd)) + 1, 3);
	bme_read_registers(BME_H6_CD, ((uint8_t*) &(bme_cd.bme_hcd.h6)), 1);

	uint8_t h_data[3] = {0};
	bme_read_registers(BME_H4_CD, h_data, 3);
	int16_t h4 = h_data[0];
	h4 = (h4 << 4) | (h_data[1] & 0xF);
	int16_t h5 = h_data[2];
	h5 = (h5 << 4) | ((h_data[1] >> 4) & 0xF);

	bme_cd.bme_hcd.h4 = h4;
	bme_cd.bme_hcd.h5 = h5;

	return;
}

// NOTE: The result is in format Q24.8 signed
static int32_t bme_get_temperature(const bme_raw_data_t* bme_raw_data) {
	int32_t temperature = 0;
	const uint8_t t_mask = (1 << bme_ctrl.osrs_t) - 1;
	temperature = (((int32_t) bme_raw_data -> t_msb) << 12) | (((int32_t) bme_raw_data -> t_lsb) << 8) | ((bme_raw_data -> t_xlsb >> 4) & t_mask);

	const int32_t dig_t1 = bme_cd.bme_tcd.t1;
	const int32_t dig_t2 = bme_cd.bme_tcd.t2;
	const int32_t dig_t3 = bme_cd.bme_tcd.t3;

	// Calibrate the previously calculated ADC data (using datasheet provided formula)
	const int32_t a = (((temperature >> 3) - (dig_t1 << 1)) * dig_t2) >> 11;
	int32_t b = (temperature >> 4) - dig_t1;
	 b = (((b * b) >> 12) * dig_t3) >> 14;

	t_fine = a + b;
	return ((t_fine * 5 + 128) >> 8);
}

// NOTE: Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
static uint32_t bme_get_pressure(const bme_raw_data_t* bme_raw_data) {
	const uint8_t p_mask = (1 << bme_ctrl.osrs_p) - 1;
	uint32_t pressure = (((uint32_t) bme_raw_data -> p_msb) << 12) | (((uint32_t) bme_raw_data -> p_lsb) << 8) | ((bme_raw_data -> p_xlsb >> 4) & p_mask);

	const int32_t dig_p1 = bme_cd.bme_pcd.p1;
	const int32_t dig_p2 = bme_cd.bme_pcd.p2;
	const int32_t dig_p3 = bme_cd.bme_pcd.p3;
	const int32_t dig_p4 = bme_cd.bme_pcd.p4;
	const int32_t dig_p5 = bme_cd.bme_pcd.p5;
	const int32_t dig_p6 = bme_cd.bme_pcd.p6;
	const int32_t dig_p7 = bme_cd.bme_pcd.p7;
	const int32_t dig_p8 = bme_cd.bme_pcd.p8;
	const int32_t dig_p9 = bme_cd.bme_pcd.p9;

	int32_t a = ((t_fine >> 1) - 64000);
	int32_t a1 = a >> 2;
	int32_t b = ((a1 * a1) >> 11) * dig_p6;
	b += (a * dig_p5) << 1;
	b = (b >> 2) + (dig_p4 << 16);
	a = (((dig_p3 * ((a1 * a1) >> 13)) >> 3) + ((dig_p2 * a) >> 1)) >> 18;
	a = ((a + 32768) * dig_p1) >> 15;

	if (a == 0) return 0; // avoid exception caused by division by zero

	pressure = ((uint32_t)(1048576 - pressure) - (b >> 12)) * 3125;
	if (pressure < 0x80000000U) pressure = (pressure << 1) / a;
	else pressure = (pressure / a) * 2;
	const int32_t p3 = pressure >> 3;
	a = (dig_p9 * ((p3 * p3) >> 13)) >> 12;
	b = (((int32_t)(pressure >> 2)) * dig_p8) >> 13;
	pressure = (uint32_t) ((int32_t) pressure + ((a + b + dig_p7) >> 4));

	return pressure;
}

// NOTE: The result is in format Q22.10 unsigned
static uint32_t bme_get_humidity(const bme_raw_data_t* bme_raw_data) {
	const int32_t dig_h1 = bme_cd.bme_hcd.h1;
	const int32_t dig_h2 = bme_cd.bme_hcd.h2;
	const int32_t dig_h3 = bme_cd.bme_hcd.h3;
	const int32_t dig_h4 = bme_cd.bme_hcd.h4;
	const int32_t dig_h5 = bme_cd.bme_hcd.h5;
	const int32_t dig_h6 = bme_cd.bme_hcd.h6;

	uint32_t humidity = ((uint32_t) bme_raw_data -> h_msb << 8) | bme_raw_data -> h_lsb;

	int32_t tmp = (t_fine - 76800);
	tmp = (((((humidity << 14) - (dig_h4 << 20) - (dig_h5 * tmp)) + 16384) >> 15) * (((((((tmp * dig_h6) >> 10) * (((tmp * dig_h3) >> 11) + 32768)) >> 10) + 2097152) * dig_h2 + 8192) >> 14));
	tmp -= ((((tmp >> 15) * (tmp >> 15)) >> 7) * dig_h1) >> 4;
	tmp = CLAMP(tmp, 0, 419430400);

	return (tmp >> 12);
}

void bme_init(void) {
	bme_get_calibration_data();
	bme_ctrl.ctrl_hum = 0x01;
	bme_ctrl.mode     = SLEEP_MODE;
	bme_ctrl.osrs_p   = 0x01;
	bme_ctrl.osrs_t   = 0x01;
	write_bme_ctrl();
	return;
}

bme_data_t bme_sample(void) {
	bme_ctrl.mode = FORCED_MODE;
	write_bme_ctrl();

	uint8_t is_data_available = 0x00;
	for (uint8_t i = 0; !is_data_available; ++i) {
		if (i == MAX_ATTEMPTS) return (bme_data_t) {0};
		bme_read_registers(BME_STATUS, &is_data_available, 1);
		is_data_available = (~is_data_available) & 0x80;
		delay_ms(500);
	}

	bme_raw_data_t bme_raw_data = {0};
	bme_read_registers(BME_DATA, (uint8_t*) &bme_raw_data, sizeof(bme_raw_data_t));

	bme_data_t bme_data = {0};
	bme_data.temperature = bme_get_temperature(&bme_raw_data);
	bme_data.pressure    = bme_get_pressure(&bme_raw_data) / 100;
	bme_data.humidity    = bme_get_humidity(&bme_raw_data) / 1024;

	return bme_data;
}

#endif //_BME_H_
