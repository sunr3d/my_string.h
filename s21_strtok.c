#include "s21_strings.h"

char* s21_strtok(char *str, const char *delim) {
  static char* last_address;
  char* token = s21_NULL;
  int token_found = 0;
  if(!str)
    str = last_address;

  return token;
}