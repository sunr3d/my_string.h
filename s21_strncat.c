#include "s21_string.h"

// Конкатенация n байт из src к dest 
char* s21_strncat(char *dest, const char *src, s21_size_t n) {
    s21_size_t dest_len = s21_strlen(dest);
    s21_size_t src_len = s21_strlen(src);

    if(n > src_len) // если в функции укажем большее количество байт ежели имеем, то n уравняем с имеющейся длиной сорса
        n = src_len;
    for(s21_size_t i = 0; i < n; i++) {
        dest[dest_len + i] = src[i];
    }
    dest[dest_len + n] = '\0';
    return dest;
}