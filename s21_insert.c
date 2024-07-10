#include "s21_string.h"

void* s21_insert(const char* src, const char* str, s21_size_t start_index) {
  // Проверка аргументов функции!!! (src && str)
  if (!src || !str) return s21_NULL;

  s21_size_t str_len = s21_strlen(str);
  s21_size_t src_len = s21_strlen(src);

  // Проверка аргументов функции!!! (start_index)
  if (src_len < start_index) return s21_NULL;

  char* res = s21_NULL;
  res = calloc((src_len + str_len + 1), sizeof(char));

  if (res) {
    s21_size_t res_index = 0;

    // strncpy src в рес до индекса
    s21_strncpy(res, src, start_index);
    res_index += start_index;

    // strncat str к res
    s21_strncat(res, str, str_len);
    res_index += str_len;

    // посимвольно остаток src в рес
    for (s21_size_t i = start_index; i < src_len; i++, res_index++) {
      res[res_index] = src[i];
    }
  }

  return res;
}