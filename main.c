#include <stdio.h>
#include "s21_string.h"
#include <string.h>

int main () {
  char dest1[] = "Th";
  char src1[] = "abobabiba";
  char dest2[] = "Th";
  char src2[] = "abobabiba";
  printf("s21_strncpy TEST 1:\n");
  printf("my_dest1 before = \"%s\"\n", dest1);
  printf("dest2 before = \"%s\"\n", dest2);
  s21_strncpy(dest1, src1, 6);
  strncpy(dest2, src2, 6);
  printf("my_dest1 = \"%s\"\n", dest1);
  printf("dest2 = \"%s\"\n", dest2);
  
  return 0;
}