#include "s21_string.h"

char* s21_strchr(const char* str, int c) {
  char* res = s21_NULL;
  while (*str && !res) {
    if (*str == c) res = (char*)str;
    str++;
  }

  if (c == '\0' && !res) res = (char*)str;

  return res;
}