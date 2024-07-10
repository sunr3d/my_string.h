#include "s21_string.h"

int s21_memcmp(const void* str1, const void* str2, s21_size_t n) {
  unsigned char* buff1 = (unsigned char*)str1;
  unsigned char* buff2 = (unsigned char*)str2;
  int res = 0;
  for (s21_size_t i = 0; i < n && !res; i++) {
    res = buff1[i] - buff2[i];
  }

  return res;
}