#include "s21_string.h"

char* s21_strstr(const char* haystack, const char* needle) {
  char* res = s21_NULL;

  s21_size_t len = s21_strlen(needle);
  if (0 == len) res = (char*)haystack;  // пустая подстрока есть в любой строке

  while (*haystack && !res) {
    if (*haystack == *needle)
      if (0 == s21_strncmp(haystack, needle, len)) res = (char*)haystack;
    haystack++;
  }

  return res;
}