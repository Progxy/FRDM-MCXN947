#ifndef _SSD1306_H_
#define _SSD1306_H_

#include <stdint.h>
#include <fpu.h>
#include <i3c.h>
#include <utils.h>
#include "atlas.h"

typedef enum {
	SSD1306_ADDRESS           = 0x3C,
	COMMAND_BYTE              = 0x00,
	PIXEL_BYTE                = 0x40,
	DISPLAY_OFF               = 0xAE,
	DISPLAY_ON                = 0xAF,
	SET_ADDRESSING_MODE       = 0x20,
	SET_COLUMN_ADDRESS        = 0x21,
	SET_CONTRAST              = 0x81,
	SET_ENTIRE_DISPLAY_OFF    = 0xA4,
	SET_MULTIPLEX             = 0xA8,
	SET_DISPLAY_CLOCKDIV      = 0xD5,
	COMSCANDEC                = 0xC8,
	SET_SEGREMAP              = 0xA0,
	SET_COMPINS               = 0xDA,
	SET_STARTLINE             = 0x40,
	SET_DISPLAY_OFFSET        = 0xD3,
	SET_NORMAL_DISPLAY        = 0xA6,
	SET_CHARGE_PUMP_REGULATOR = 0x8D,
	SET_PRECHARGE             = 0xD9,
	DEACTIVATE_SCROLL         = 0x2E,
	SET_VCOMDETECT            = 0xDB,
	SET_PAGE_ADDRESS          = 0x22
} DisplayCommands;

typedef enum {
	DISPLAY_HEIGHT = 64,
	DISPLAY_WIDTH  = 128,
	DISPLAY_COLS   = 128,
	DISPLAY_PAGES  = 8,
	MAX_DIGIT_LEN  = 12
} DisplayConstants;

static uint8_t ssd1306_display_buf[DISPLAY_COLS * DISPLAY_PAGES] = {0};
static uint32_t ssd1306_display_idx = 0;

static void ssd1306_write_pixel_buf(const uint8_t* data, size_t count) {
	while (count) {
		uint8_t buf[16] = { PIXEL_BYTE };
		memcpy(buf + 1, data, MIN(count, 15));
		int status = __send_i2c(I3C1, SSD1306_ADDRESS, buf, MIN(count + 1, 16), TRUE);
		if (status) __asm__("bkpt");
		count -= status;
		data += status;
	}
	return;
}

static void ssd1306_send_command(const uint8_t cmd, const uint8_t* data, const size_t count) {
	uint8_t buf[16] = { COMMAND_BYTE, cmd };
	if (count > 0 || data == NULL) memcpy(buf + 2, data, MIN(count, 14));
	int status = __send_i2c(I3C1, SSD1306_ADDRESS, buf, MIN(count + 2, 16), TRUE);
	if (status) __asm__("bkpt");
	return;
}

static void ssd1306_set_offset(uint8_t page, uint8_t col) {
	const uint8_t cols[2]  = {col  & 0x7F, 0x7F};
	const uint8_t pages[2] = {page & 0x07, 0x07};
	ssd1306_send_command(SET_PAGE_ADDRESS, pages, 2);
	ssd1306_send_command(SET_COLUMN_ADDRESS, cols, 2);
	return;
}

void ssd1306_display(void) {
	ssd1306_set_offset(0, 0);
	ssd1306_write_pixel_buf(ssd1306_display_buf, sizeof(ssd1306_display_buf));
	return;
}

void ssd1306_init(void) {
	ssd1306_send_command(DISPLAY_OFF, NULL, 0);

	uint8_t data = 63;
	ssd1306_send_command(SET_MULTIPLEX, &data, 1);

	data = 0x00;
	ssd1306_send_command(SET_DISPLAY_OFFSET, &data, 1);

	ssd1306_send_command(SET_STARTLINE, NULL, 0);
	ssd1306_send_command(SET_SEGREMAP | 1, NULL, 0);
	ssd1306_send_command(COMSCANDEC, NULL, 0); //?

	data = 0x12;
	ssd1306_send_command(SET_COMPINS, &data, 1);

	data = 0x7F;
	ssd1306_send_command(SET_CONTRAST, &data, 1);

	ssd1306_send_command(SET_NORMAL_DISPLAY, NULL, 0);

	data = 0x80;
	ssd1306_send_command(SET_DISPLAY_CLOCKDIV, &data, 1);

	data = 0x14;
	ssd1306_send_command(SET_CHARGE_PUMP_REGULATOR, &data, 1);

	data = 0xF1;
	ssd1306_send_command(SET_PRECHARGE, &data, 1);

	data = 0x00;
	ssd1306_send_command(SET_ADDRESSING_MODE, &data, 1);

	data = 0x40;
	ssd1306_send_command(SET_VCOMDETECT, &data, 1);

	ssd1306_send_command(SET_ENTIRE_DISPLAY_OFF, NULL, 0);
	ssd1306_send_command(DEACTIVATE_SCROLL, NULL, 0);
	ssd1306_send_command(DISPLAY_ON, NULL, 0);

	ssd1306_display();

	return;
}

static void ssd1306_next_row(void) {
	ssd1306_display_idx += (DISPLAY_WIDTH - (ssd1306_display_idx % DISPLAY_WIDTH));
	ssd1306_display_idx %= sizeof(ssd1306_display_buf);
	return;
}

void ssd1306_print_char(const char c) {
	if ((ssd1306_display_idx % DISPLAY_WIDTH) + sizeof(*font_atlas) > DISPLAY_WIDTH) {
		ssd1306_display_idx += (DISPLAY_WIDTH - (ssd1306_display_idx % DISPLAY_WIDTH));
	}

	if (ssd1306_display_idx + sizeof(*font_atlas) >= sizeof(ssd1306_display_buf)) return;
	else if (c == '\n') {
		ssd1306_next_row();
		return;
	}

	memcpy(ssd1306_display_buf + ssd1306_display_idx, font_atlas[(c - 0x20) % sizeof(font_atlas)], sizeof(*font_atlas));
	ssd1306_display_idx = (ssd1306_display_idx + sizeof(*font_atlas)) % sizeof(ssd1306_display_buf);
	return;
}

void ssd1306_print_str(const char* str) {
	while (*str != '\0') ssd1306_print_char(*str++);
	return;
}

void ssd1306_clear_display(void) {
	ssd1306_display_idx = 0;
	memset(ssd1306_display_buf, 0, sizeof(ssd1306_display_buf));
	return;
}

void ssd1306_display_rand_quote(void) {
    const char* weather_quotes[6] = {
        // 1. Stevenson
        "\"THERE IS NO     \nBAD WEATHER,ONLY \nGOOD CLOTHES-RLS\"",
        // 2. Charles Warner
        "\"EVERYONE TALKS  \nABOUT WEATHER..  \nBUT NO ACTION-CW\"",
        // 3. John Ruskin
        "\"SUN IS DELIGHT  \nRAIN REFRESHING  \n-JR        \\o/ \"",
        // 4. Bob Marley
        "\"SOME PEOPLE     \nFEEL THE RAIN..  \n-BM      ( ._.) \"",
        // 5. Henry Wadsworth Longfellow
        "\"INTO EACH LIFE  \nSOME RAIN MUST   \nFALL.-HWL ( ._.)\""
    };

    int random_pick = random(5);
    ssd1306_print_str(weather_quotes[random_pick]);

    return;
}

#endif //_SSD1306_H_
