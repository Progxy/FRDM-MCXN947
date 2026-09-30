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

#endif //_UTILS_H