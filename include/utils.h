#ifndef _UTILS_H_
#define _UTILS_H_

#define TRUE 1
#define FALSE 0
#define UNUSED_FUNCTION __attribute__((unused))

UNUSED_FUNCTION static void memcpy(void* dest, void* src, int size) {
  if (dest == NULL || src == NULL) return;
  unsigned char* dest_b = (unsigned char*) dest;
  unsigned char* src_b = (unsigned char*) src;
  while (size--) *dest_b++ = *src_b++;
  return;
}

UNUSED_FUNCTION static void memset(void* dest, unsigned char value, int size) {
  if (dest == NULL) return;
  unsigned char* dest_b = (unsigned char*) dest;
  while (size--) *dest_b++ = value;
  return;
}

// TODO: Substitute it with a timer callback
UNUSED_FUNCTION static void delay(unsigned int ms) {
  for (unsigned int j = 0; j < ms; ++j) {
	  for (unsigned int i = 0; i < 10000; ++i) {
	 	__asm volatile("nop");
	  }
	}
  return;
}

#endif //_UTILS_H
