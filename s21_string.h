#ifndef S21_STRING_H
#define S21_STRING_H

#define s21_NULL ((void*)0)

typedef unsigned long long s21_size_t;

// Ищет в str появления символа "с", в первых n байтах строки str
void* s21_memchr(const void* str, int c, s21_size_t n);

// Сравнивает первые n байт str1 и str2 (любые массивы)
int s21_memcmp(const void* str1, const void* str2, s21_size_t n);

// Копирует n байт из src в dest (любые массивы)
void* s21_memcpy(void* dest, const void* src, s21_size_t n);

// Заносит символ "c" в первые n байт строки str 
void* s21_memset(void* str, int c, s21_size_t n);

// Дополняет строку dest из строки src в размере n байт
char* s21_strncat(char* dest, const char* src, s21_size_t n);

// Ищет первое появление символа 'с' в строке str и возвращает поинтер на это место
char* s21_strchr(const char* str, int c);

// Сравнивает первые n символов str1 и str2 (ТОЛЬКО СТРОКИ)
int s21_strncmp(const char* str1, const char* str2, s21_size_t n);

// Копирует n символов из src в dest (ТОЛЬКО СТРОКИ)
char* s21_strncpy(char *dest, const char *src, s21_size_t n);

// Высчитывает длину строки str1 ДО появление любого символа из строки str2
s21_size_t s21_strcspn(const char *str1, const char *str2); 

// Возвращает указатель на строку с наименованием ошибки
char* s21_strerror(int errnum);

// Возвращает длину строки до терминатора, но не считая его самого
s21_size_t s21_strlen(const char *str);

// Возвращает указатель на первый найденный символ в строке str1 из строки str2
char* s21_strpbrk(const char *str1, const char *str2);

// Возвращает указатель на ПОСЛЕДНЕЕ вхождение символа 'c' в строке str
char* s21_strrchr(const char *str, int c);

// Возвращает указатель на ПОДСТРОКУ needle в строке haystack
char* s21_strstr(const char *haystack, const char *needle);

// Разделяет строку str на токены по символам из delim. Возвращает указатель на следующий токен.
char* s21_strtok(char *str, const char *delim);

int s21_sscanf(const char* str, const char* format, ...);
int s21_sprintf(char* str, const char* format, ...);

// Переводит все буквы в верхний регистр
void* s21_to_upper(const char *str);

// Переводит все буквы в нижний регистр
void* s21_to_lower(const char *str);

// 
void* s21_insert(const char *src, const char *str, s21_size_t start_index);

//
void* s21_trim(const char *src, const char *trim_chars);

#endif // S21_STRING_H