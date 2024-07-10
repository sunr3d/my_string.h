#include "s21_string.h"

// Все буквы строки в ловеркейс
void* s21_to_lower(const char* str) {
  char* buff = s21_NULL;
  // s21_size_t str_len = s21_strlen(str);

  if (str) buff = malloc((s21_strlen(str) + 1) * sizeof(char));

  if (buff) {
    s21_strncpy(buff, str, s21_strlen(str) + 1);
    for (s21_size_t i = 0; i < s21_strlen(buff); i++) {
      if (buff[i] >= 'A' && buff[i] <= 'Z') buff[i] += 32;
    }
  }

  return buff;
}