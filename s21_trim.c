#include "s21_string.h"

void* s21_trim(const char* src, const char* trim_chars) {
  if (!src || !trim_chars) return s21_NULL;

  char* res = s21_NULL;
  res = calloc((s21_strlen(src) + 1), sizeof(char));

  if (res && *trim_chars != '\0') {
    while (*src && s21_strchr(trim_chars, *src)) {
      src++;
    }
    res = s21_strncpy(res, src, s21_strlen(src));
    int flag = 0;
    for (int i = s21_strlen(res) - 1; i >= 0 && !flag; i--) {
      if (s21_strchr(trim_chars, res[i]))
        res[i] = '\0';
      else
        flag = 1;
    }
  } else if (res && *trim_chars == '\0') {
    res = s21_strncpy(res, src, s21_strlen(src));
  }

  return res;
}