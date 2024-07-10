CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11
S21_C_FILES=s21_*.c
S21_OBJECTS=s21_*.o
CLEAN_TESTS=rm -rf tests.c tests tests_gcov
CLEAN_OBJECTS=rm -rf *.o *.so *.out *.gcda *.gcno *.info

# Прогон ликов в зависимости от ОСи
OS = $(shell uname)
ifeq ("$(OS)","Linux")
	LEAKS_TEST = valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all --log-file="leaks_tests.log" ./tests
else
	LEAKS_TEST = leaks -atExit -- ./tests > leaks_tests.log
endif

all: s21_string.a
	$(CLEAN_OBJECTS)

s21_string.a: s21_objects
	ar rcs s21_string.a $(S21_OBJECTS) 

s21_string_gcov.a: s21_objects_gcov
	ar rcs s21_string_gcov.a $(S21_OBJECTS)

s21_objects:
	$(CC) $(CFLAGS) -g -c $(S21_C_FILES)

s21_objects_gcov:
	$(CC) $(CFLAGS) --coverage -c $(S21_C_FILES)

tests.c: tests.check
	checkmk clean_mode=1 tests.check > tests.c

sprintf_tests.c: sprintf_tests.check
	checkmk clean_mode=1 sprintf_tests.check > tests.c

tests.o: tests.c s21_string.h
	$(CC) $(CFLAGS) -g -c tests.c

test: s21_string.a tests.o
	$(CC) $(CFLAGS) -g tests.o -o tests `pkg-config --cflags --libs check` -L. s21_string.a -lm
	./tests
	$(CLEAN_TESTS)
	$(CLEAN_OBJECTS)

leak_test: s21_string.a tests.o
	$(CC) $(CFLAGS) -g tests.o -o tests `pkg-config --cflags --libs check` -L. s21_string.a -lm
	$(LEAKS_TEST)
	$(CLEAN_TESTS)
	$(CLEAN_OBJECTS)

gcov_report: s21_string_gcov.a tests.o
	$(CC) $(CFLAGS) tests.o -o tests_gcov `pkg-config --cflags --libs check` s21_string_gcov.a --coverage -lm
	./tests_gcov
	mkdir -p report
	gcovr --html-details report/index.html --html-medium-threshold 80 --html-high-threshold 100 --html-title "S21_STRINGS GCOV REPORT"
	$(CLEAN_TESTS)
	$(CLEAN_OBJECTS)

clean:
	$(CLEAN_OBJECTS) $(CLEAN_TESTS) *.a *.log report

clang:
	clang-format -i --style=Google *.c *.h
	clang-format -n --style=Google *.c *.h

rebuild: clean all