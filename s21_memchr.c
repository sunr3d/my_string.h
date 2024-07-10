#include "s21_string.h"

void* s21_memchr(const void* str, int c, s21_size_t n) {
  void* res = s21_NULL;  // результативный поинтер
  unsigned char* buff = (unsigned char*)str;  // явно приводим буфер к u char
  for (s21_size_t i = 0; i < n && !res;
       i++) {  // циклом идем до конца указаного кол-ва байт, либо пока не
               // найдем адрес символа что мы ищем
    if (buff[i] == c) res = &buff[i];
  }

  return res;
}