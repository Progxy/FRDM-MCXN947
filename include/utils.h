#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

#define TRUE 1
#define FALSE 0
#define UNUSED_FUNCTION __attribute__((unused))

UNUSED_FUNCTION static void memcpy(void* dest, void* src, int size) {
  if (dest == NULL || src == NULL) return;
  uint8_t* dest_b = (uint8_t*) dest;
  uint8_t* src_b  = (uint8_t*) src;
  while (size--) *dest_b++ = *src_b++;
  return;
}

UNUSED_FUNCTION static void memset(void* dest, uint8_t value, int size) {
  if (dest == NULL) return;
  uint8_t* dest_b = (uint8_t*) dest;
  while (size--) *dest_b++ = value;
  return;
}

// TODO: Substitute it with a timer callback and maybe add a delay in microseconds function
UNUSED_FUNCTION static void delay_ms(unsigned int ms) {
	for (unsigned int j = 0; j < ms; ++j) {
		for (unsigned int i = 0; i < 10000; ++i) {
			__asm volatile("nop");
		}
	}
	return;
}

UNUSED_FUNCTION static inline uint32_t mask_lower_bits(uint32_t val, const uint8_t bits) {
	return val & ~((1U << bits) - 1);
}

UNUSED_FUNCTION static inline uint32_t mask_upper_bits(uint32_t val, const uint8_t bits) {
	return val & ((1U << bits) - 1);
}

#endif //_UTILS_H
