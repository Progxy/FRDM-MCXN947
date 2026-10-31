#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0
#define UNUSED_FUNCTION __attribute__((unused))
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))

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
		for (unsigned int i = 0; i < 10000; ++i) __asm__ volatile("nop");
	}
	return;
}

UNUSED_FUNCTION static void delay_us(unsigned int us) {
	for (unsigned int j = 0; j < us; ++j) {
		for (unsigned int i = 0; i < 10; ++i) __asm__ volatile("nop");
	}
	return;
}

UNUSED_FUNCTION static inline uint32_t mask_lower_bits(uint32_t val, const uint8_t bits) {
	return val & ~((1U << bits) - 1);
}

UNUSED_FUNCTION static inline uint32_t mask_upper_bits(uint32_t val, const uint8_t bits) {
	return val & ((1U << bits) - 1);
}

UNUSED_FUNCTION static inline uint32_t __abs(int32_t val) {
	return val >= 0 ? val : -val;
}

#include <stdarg.h>
#define MAX_NUM_LEN 15
#define MAX_BIT_NUM_LEN 32
#define IS_A_HEX_VAL(chr)     (IS_A_NUM(chr) || (IS_LOWER_CASE(chr) && (chr) <= 'f') || (IS_UPPER_CASE(chr) && (chr) <= 'F'))
#define CHR_TO_HEX(chr)       (IS_LOWER_CASE(chr) ? ((chr) - 'a' + 10) : ((chr) - 'A' + 10))
#define HEX_TO_CHR_CAP(val)   ((val) > 9 ? (val) + 55 : (val) + '0')
#define HEX_TO_CHR(val)       ((val) > 9 ? (val) + 87 : (val) + '0')
#define IS_A_NUM(chr)         (((chr) >= '0') && ((chr) <= '9'))
#define IS_LOWER_CASE(chr)    ((chr) >= 'a' && (chr) <= 'z')
#define IS_UPPER_CASE(chr)    ((chr) >= 'A' && (chr) <= 'Z')
#define GET_BIT(val, bit_pos) (((val) >> (bit_pos)) & 0x01)
#define BITS_SIZE(val)        (sizeof(val) * 8)
#define NUM_TO_CHR(val)       ((val) + '0')

UNUSED_FUNCTION static uint32_t strlen(const char* str) {
    if (str == NULL) return 0;
    const char* str_c = str;
	while (*str++);
    return (uint32_t) (str - str_c - 1);
}

UNUSED_FUNCTION static char* strcpy(char* dest, const char* src) {
	if (dest == NULL || src == NULL) return NULL;
	char* const o_dest = dest;
  	while ((*dest++ = *src++));
	return o_dest;
}

UNUSED_FUNCTION int starts_with(const char* str, const char* pattern) {
	if (str == NULL || pattern == NULL) return FALSE;
	for (; *str && *pattern; ++str, ++pattern) {
		if (*str != *pattern) return FALSE;
	}
	if (*str == '\0' && *pattern != '\0') return FALSE;
	return TRUE;
}

UNUSED_FUNCTION static char* reverse_str(char* str) {
    const uint32_t len = strlen(str);
    for (uint32_t i = 0; i < (len / 2); ++i) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

// TODO: Ignoring that data is in little-endian
UNUSED_FUNCTION static uint32_t to_hex_str(char* str, const uint8_t* byte_str, const uint32_t byte_size) {
    char* const str_base = str;
	for (int j = byte_size - 1; j >= 0; --j) {
        *str++ = HEX_TO_CHR_CAP((byte_str[j] >> 4) & 0xF);
        *str++ = HEX_TO_CHR_CAP(byte_str[j] & 0xF);
    }
    *str = '\0';
    return (str - str_base);
}

UNUSED_FUNCTION static uint32_t to_dec_str(char* str, const uint8_t* byte_str, const uint32_t byte_size) {
	uint32_t val = 0;
	for (uint8_t i = byte_size; i > 0; --i) {
		val *= 256;
		val += byte_str[i - 1];
	}

	char* const str_base = str;
	while (val) {
		uint8_t value = val % 10;
		*str++ = value + '0';
		val = (val - (val % 10)) / 10;
	}

	*str = '\0';
    reverse_str(str_base);

	return (str - str_base);
}

// TODO: Ignoring that data is in little-endian
UNUSED_FUNCTION static uint32_t to_bit_str(char* str, const uint8_t* byte_str, const uint32_t byte_size) {
	char* const str_base = str;
    for (int j = byte_size - 1; j >= 0; --j) {
		for (int i = 7; i >= 0; --i) {
			*str++ = ((byte_str[j] >> i) & 0x01) + '0';
		}
	}
    *str = '\0';
    return (str - str_base);
}

UNUSED_FUNCTION static uint32_t to_oct_str(char* str, const uint8_t* byte_str, const uint32_t byte_size) {
	char* const str_base = str;
	unsigned int bit_pos = 0;
	unsigned int byte_pos = 0;
	while (byte_pos < byte_size) {
		uint8_t temp = 0;
		for (unsigned int j = 0; j < 3 && byte_pos < byte_size; ++j, ++bit_pos) {
			if (bit_pos > 7) byte_pos++, bit_pos = 0;
			temp |= (0x01 & (byte_str[byte_pos] >> bit_pos)) << j;
		}
		*str++ = NUM_TO_CHR(temp & 0x07);
	}
    *str = '\0';
    reverse_str(str_base);
    return (str - str_base);
}

__attribute__((unused, format(printf, 3, 4))) static int snprintf(char* str, size_t size, const char* format, ...);
UNUSED_FUNCTION static int vsnprintf(char* str, size_t size, const char* format, va_list args);

__attribute__((unused, format(printf, 3, 4))) static int snprintf(char* str, size_t size, const char* format, ...) {
    va_list args;
    va_start(args, format);
	int ret = vsnprintf(str, size, format, args);
    va_end(args);
    return ret;
}

UNUSED_FUNCTION static int vsnprintf(char* str, size_t size, const char* format, va_list args) {
	if (str == NULL || format == NULL) return -1;

	size_t format_size = strlen(format);
	size_t str_index = 0;

	for (size_t i = 0; (i <= format_size) && (str_index < size); ++i) {
		if (starts_with((char*) format + i, "%%")) {
			str[str_index] = '%';
			str_index++;
			++i;
		} else if (starts_with((char*) format + i, "%c")) {
			char var_chr = (char) va_arg(args, int);
			str[str_index] = var_chr;
			str_index++;
			++i;
		} else if (starts_with((char*) format + i, "%s")) {
			char* var_str = va_arg(args, char*);
			if (var_str == NULL) var_str = "(null)";
			if (strlen(var_str) + str_index >= size) return str_index;
			strcpy(str + str_index, var_str);
			str_index += strlen(var_str);
			++i;
		} else if (starts_with((char*) format + i, "%p")) {
			if (2 + str_index >= size) return str_index;
			str[str_index++] = '0';
			str[str_index++] = 'x';
			if (MAX_NUM_LEN + str_index >= size) return str_index;
			uint32_t var_ptr = (uint32_t) va_arg(args, void*);
			str_index += to_hex_str(str + str_index, (uint8_t*)&var_ptr, sizeof(var_ptr));
			++i;
		} else if (starts_with((char*) format + i, "%d") || starts_with((char*) format + i, "%i")) {
			int var_int = va_arg(args, int);
			if (var_int < 0) {
				if (1 + str_index >= size) return str_index;
				str[str_index++] = '-';
			}
			if (MAX_NUM_LEN + str_index >= size) return str_index;
			unsigned int var_uint = __abs(var_int);
			str_index += to_dec_str(str + str_index, (uint8_t*)&var_uint, sizeof(var_uint));
			++i;
		} else if (starts_with((char*) format + i, "%u")) {
			if (MAX_NUM_LEN + str_index >= size) return str_index;
			unsigned int var_uint = va_arg(args, unsigned int);
			str_index += to_dec_str(str + str_index, (uint8_t*)&var_uint, sizeof(var_uint));
			++i;
		} else if (starts_with((char*) format + i, "%X") || starts_with((char*) format + i, "%x")) {
			if (2 + str_index >= size) return str_index;
			str[str_index++] = '0';
			str[str_index++] = 'x';
			if (MAX_NUM_LEN + str_index >= size) return str_index;
			unsigned int var_uint = va_arg(args, unsigned int);
			str_index += to_hex_str(str + str_index, (uint8_t*)&var_uint, sizeof(var_uint));
			++i;
		} else if (starts_with((char*) format + i, "%b")) {
			if (2 + str_index >= size) return str_index;
			str[str_index++] = '0';
			str[str_index++] = 'b';
			if (MAX_BIT_NUM_LEN + str_index >= size) return str_index;
			unsigned int var_uint = va_arg(args, unsigned int);
			str_index += to_bit_str(str + str_index, (uint8_t*)&var_uint, sizeof(var_uint));
			++i;
		} else if (starts_with((char*) format + i, "%o")) {
			if (1 + str_index >= size) return str_index;
			str[str_index++] = '0';
			if (MAX_NUM_LEN + str_index >= size) return str_index;
			unsigned int var_uint = va_arg(args, unsigned int);
			str_index += to_oct_str(str + str_index, (uint8_t*)&var_uint, sizeof(var_uint));
			++i;
		} else {
			str[str_index] = format[i];
			str_index++;
		}
	}

	return str_index;
}

#endif //_UTILS_H
