#include "s21_string.h"

char* s21_strncpy(char* dest, const char* src, s21_size_t n) {
  char* b_dest = dest;
  char* b_src = (char*)src;
  while (n > 0 && *b_src != '\0') {
    *b_dest = *b_src;
    b_dest++;
    b_src++;
    n--;
  }

  while (n > 0 && b_dest != s21_NULL) {
    *b_dest = '\0';
    b_dest++;
    n--;
  }

  return dest;
}
