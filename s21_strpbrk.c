#include "s21_string.h"

char* s21_strpbrk(const char* str1, const char* str2) {
  char* res = s21_NULL;
  while (*str1 && !res) {
    if (s21_strchr(str2, *str1)) res = (char*)str1;
    str1++;
  }

  return res;
}