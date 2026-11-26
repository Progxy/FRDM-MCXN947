#ifndef _ELS_H_
#define _ELS_H_

#include <utils.h>

static const unsigned int els_base_addr = 0x40054000;
const unsigned int els_status_addr = 0x00;
const unsigned int els_ctrl_addr = 0x04;
const unsigned int els_cmd_cfg0_addr = 0x08;
const unsigned int els_dma_res0_addr = 0x38;
const unsigned int els_dma_res0_len_addr = 0x3C;
const unsigned int els_err_status = 0x4C;
const unsigned int els_err_status_clr_addr = 0x50;
const unsigned int els_prng_datout_addr = 0x5C;

typedef enum { ELS_OK = 0, ELS_INIT_FAIL = 1, ELS_OP_ERR = 2 } ELSError;
typedef enum { CIPHER = 0, AUTH_CIPHER, ECSIGN = 4, ECVFY, ECKXCH, KEYGEN = 8, KEYIN, KEYOUT, KDELETE, CKDF = 16, HKDF, TLS_INIT, HASH, HMAC, CMAC, RND_REQ = 24, DRBG_TEST, DTRNG_CFG_LOAD = 28 } ELSCommands;

typedef struct {
	uint8_t els_busy: 1;
	uint8_t els_irq: 1;
	uint8_t els_err: 1;
	uint8_t prng_rdy: 1;
	uint8_t ecdsa_vfy_status: 2;
	uint8_t pprot: 2;
	uint8_t drbg_ent_lvl: 2;
	uint8_t dtrng_busy: 1;
	uint8_t rsv: 5;
	uint8_t els_lock: 1;
	uint16_t rsv1: 15;
} __attribute__((packed)) els_status_t;

typedef struct {
	uint8_t els_en: 1;
	uint8_t els_start: 1;
	uint8_t els_reset: 1;
	uint8_t els_cmd: 5;
	uint8_t byte_order: 1;
	uint8_t rsv: 7;
	uint16_t rsv1;
} __attribute__((packed)) els_ctrl_t;

static_assert(sizeof(els_status_t) == 4, "Size must be 4 bytes");
static_assert(sizeof(els_ctrl_t) == 4, "Size must be 4 bytes");

__attribute__((constructor(105))) int init_els(void) {
	volatile uint32_t* els_ctrl_reg = (uint32_t*) (els_base_addr + els_ctrl_addr);
	els_ctrl_t els_ctrl = *(els_ctrl_t*)els_ctrl_reg;
	els_ctrl.els_en = 1;
	*els_ctrl_reg = *((uint32_t*)&els_ctrl);

	volatile els_status_t* els_status = (els_status_t*) (els_base_addr + els_status_addr);
	while (els_status -> els_busy) delay_us(1);

	return 0;
}

int els_handle_err(void) {
	// TODO: Read precisely which error has been triggered and return it
	volatile els_status_t* els_status = (els_status_t*) (els_base_addr + els_status_addr);
	while (els_status -> els_busy) delay_us(1);
	volatile uint32_t* els_err_status_clr = (uint32_t*) (els_base_addr + els_err_status_clr_addr);
	*els_err_status_clr |= 0x01;
	while (els_status -> els_err) delay_us(1);
	return -ELS_OP_ERR;
}

int rnd_req(const void* res_ptr, const uint16_t size, const uint8_t low_ent_init, const uint8_t rnd_raw) {
	volatile uint32_t* els_ctrl_reg = (uint32_t*) (els_base_addr + els_ctrl_addr);
	volatile uint32_t* els_dma_res0 = (uint32_t*) (els_base_addr + els_dma_res0_addr);
	volatile uint32_t* els_dma_res0_len = (uint32_t*) (els_base_addr + els_dma_res0_len_addr);
	volatile uint32_t* els_cmd_cfg0 = (uint32_t*) (els_base_addr + els_cmd_cfg0_addr);

	*els_dma_res0 = (uint32_t)res_ptr;
	if (low_ent_init == 0 || rnd_raw == 0) *els_dma_res0_len = size;
	*els_cmd_cfg0 = (low_ent_init & 0x01) | ((rnd_raw & 0x01) << 1);

	els_ctrl_t els_ctrl = *(els_ctrl_t*)els_ctrl_reg;
	els_ctrl.els_cmd = RND_REQ;
	els_ctrl.els_start = 1;
	*els_ctrl_reg = *((uint32_t*)&els_ctrl);

	volatile els_status_t* els_status = (els_status_t*) (els_base_addr + els_status_addr);
	while (els_status -> els_busy && !els_status -> els_err) delay_us(1);
	if (els_status -> els_err) return els_handle_err();

	return 0;
}

uint32_t rand(int* err) {
	*err = 0;
	volatile uint32_t* els_ctrl_reg = (uint32_t*) (els_base_addr + els_ctrl_addr);
	if ((*els_ctrl_reg & 0x01) == 0) __bkpt();

	volatile els_status_t* els_status = (els_status_t*) (els_base_addr + els_status_addr);
	els_status_t s = *els_status;
	// TODO: somehow cannot dereference directly this field without getting a bus fault
	if (s.drbg_ent_lvl == 0) {
		uint32_t temp = 0;
		if (rnd_req(&temp, 4, 1, 0) != 0) {
			*err = -ELS_OP_ERR;
			return 0;
		}
	}

	while (!els_status -> prng_rdy) delay_us(1);
	return *(uint32_t*) (els_base_addr + els_prng_datout_addr);
}

__attribute__((section(".after_vectors.els_handler"), naked, noreturn))
void els_handler(void) {
	__bkpt();
	__asm__("bx lr");
}

#endif //_ELS_H_
