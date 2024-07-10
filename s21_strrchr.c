#include "s21_string.h"

char* s21_strrchr(const char* str, int c) {
  char* res = s21_NULL;
  while (*str) {
    if (*str == c) res = (char*)str;
    str++;
  }

  if (c == '\0' && !res) res = (char*)str;

  return res;
}