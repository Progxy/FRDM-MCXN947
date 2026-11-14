#ifndef _SCD_H_
#define _SCD_H_

#include <stdint.h>
#include <fpu.h>
#include <utils.h>

typedef enum {
	SCD_ADDRESS                    = 0x62,
	SERIAL_NUMBER                  = 0x3682,
	SENSOR_VARIANT                 = 0x202F,
	DATA_READY_STATUS              = 0xE4B8,
	MEASURE_SINGLE_SHOT            = 0x219D,
	READ_MEASUREMENT               = 0xEC05,
	LOW_POWER_PERIODIC_MEASUREMENT = 0x21AC
} SCDAddresses;

typedef struct {
	union {
		struct {
			uint8_t data_msb;
			uint8_t data_lsb;
		} __attribute__((packed));
		uint8_t data[2];
	};
	uint8_t crc;
} __attribute__((packed)) scd_response_t;

static_assert(sizeof(scd_response_t) == 3, "Size must be 3 uint8_ts.");

typedef struct {
	uint16_t c02;
	int16_t t;
	uint16_t rh;
} scd_data_t;

#define scd_read_address(addr, dest, cnt) __scd_read_address((addr), (dest), (cnt), 1)
void __scd_read_address(uint16_t addr, scd_response_t* dest, uint8_t cnt, uint16_t exec_time_ms) {
	uint8_t buf[3] = {0};
	buf[0] = (uint8_t) ((addr & 0xFF00) >> 8);
	buf[1] = (uint8_t) ((addr & 0x00FF) >> 0);

	int status = __send_i2c(I3C1, SCD_ADDRESS, buf, 2, TRUE);
	if (status) __asm__("bkpt");

	delay_ms(exec_time_ms);

	if (cnt == 0 || dest == NULL) return;
	else if (cnt > 5) __asm__("bkpt");

	status = __recv_i2c(I3C1, SCD_ADDRESS, dest, cnt * sizeof(scd_response_t), TRUE);
	if (status) __asm__("bkpt");

	return;
}

uint8_t scd_calculate_crc(const uint8_t* data, const uint16_t size) {
	uint8_t crc = 0xFF;
	for (int16_t i = size - 1; i >= 0; --i) {
		crc ^= data[i];
		for (uint8_t j = 8; j > 0; --j) {
			if (crc & 0x80) crc = (crc << 1) ^ 0x31;
			else crc <<= 1;
		}
	}
	return crc;
}

bool scd_check_crc(scd_response_t response) {
	const uint8_t check = scd_calculate_crc(response.data, 2);
	return (check != response.crc);
}

bool scd_get_ready_status(void) {
	scd_response_t status = {0};
	scd_read_address(DATA_READY_STATUS, &status, 1);
	uint16_t data = ((uint16_t) status.data_msb << 8) | status.data_lsb;
	return !!(data & 0xFFF);
}

scd_data_t scd_read_measurement(void) {
	if (!(scd_get_ready_status())) return (scd_data_t) {0};

	scd_response_t measurement[3] = {0};
	scd_read_address(READ_MEASUREMENT, measurement, 3);

	if (scd_check_crc(measurement[0])) __asm__("bkpt"); //Serial.println("Failed CRC check on C02");
	if (scd_check_crc(measurement[1])) __asm__("bkpt"); //Serial.println("Failed CRC check on Temperature");
	if (scd_check_crc(measurement[2])) __asm__("bkpt"); //Serial.println("Failed CRC check on RH");

	uint16_t data[3] = {0};
	for (unsigned int i = 0; i < 3; ++i) {
		data[i] = ((uint16_t) measurement[i].data_msb << 8) | measurement[i].data_lsb;
	}

	scd_data_t scd_data = {0};
	scd_data.c02 = data[0];
	scd_data.t   = -45 + 175 * (((float) data[1]) / 0xFFFF);
	scd_data.rh  =       100 * (((float) data[2]) / 0xFFFF);

	return scd_data;
}

void scd_init(void) {
	delay_ms(30); // Wait for power-up
	return;
}

scd_data_t scd_sample(void) {
	__scd_read_address(MEASURE_SINGLE_SHOT, NULL, 0, 5000);
	return scd_read_measurement();
}

#endif //_SCD_H_
