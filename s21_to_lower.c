#include "s21_strings.h"

// Все буквы строки в ловеркейс
void* s21_to_lower(const char *str) {
    char* buff = s21_NULL;
    s21_size_t str_len = s21_strlen(str);

    if(str)
        buff = malloc((str_len + 1) * sizeof(char));
    
    if(buff) {
        s21_strncpy(buff, str, str_len + 1);
        for(s21_size_t i = 0; i < str_len; i++) {
            if(buff[i] >= 'A' && buff[i] <= 'Z')
                buff[i] += 32;
        }
    }

    return buff;
}