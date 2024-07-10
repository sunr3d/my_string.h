#include "s21_string.h"

void* s21_to_upper(const char* str) {
  char* buff = s21_NULL;

  if (str) buff = malloc((s21_strlen(str) + 1) * sizeof(char));

  if (buff) {
    s21_strncpy(buff, str, s21_strlen(str) + 1);
    for (s21_size_t i = 0; i < s21_strlen(buff); i++) {
      if (buff[i] >= 'a' && buff[i] <= 'z') buff[i] -= 32;
    }
  }

  return buff;
}