#include "s21_string.h"

void* s21_memcpy(void* dest, const void* src, s21_size_t n) {
  unsigned char* b_dest = (unsigned char*)dest;
  const unsigned char* b_src = (const unsigned char*)src;
  while (n > 0) {
    *b_dest = *b_src;
    b_dest++;
    b_src++;
    n--;
  }

  return dest;
}