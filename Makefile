FLAGS=-std=c11 -c -g -Wall -Wextra -Werror
FILE=s21_convertors_functions.c s21_helpful_functions.c s21_another_functions.c s21_comparison_functions.c s21_arithmetic_functions.c
LIBS_MACOS= -L /usr/local/lib -lcheck
LIBS_LINUX= -lcheck -lsubunit -lpthread -lrt -lm -D_GNU_SOURCE
VAR = $(shell pkg-config --cflags --libs check)

OS = $(shell uname)
ifeq ($(OS), Darwin)
OPEN=open gcov_report/index.html
LIBS=$(LIBS_MACOS)
LEAK_CHECK = leaks --atExit -- ./test
endif

ifeq ($(OS), Linux)
OPEN=open gcov_report/index.html
LIBS=$(LIBS_LINUX)
LEAK_CHECK = valgrind --leak-check=full --show-leak-kinds=all -s ./test
endif

all: clean s21_decimal.a

test: clean s21_decimal.a
	gcc $(FLAGS) s21_test.c $(VAR) -c
	gcc -o test *.o $(LIBS)
	./test

s21_decimal.a:
	gcc $(FILE) $(VAR) -c $(FLAGS)
	ar rcs s21_decimal.a s21*.o
	cp s21_decimal.a libs21_decimal.a

gcov_report: clean
	gcc -c $(FLAGS) $(FILE) --coverage
	ar rcs gcov_s21_decimal.a *.o
	
	gcc -c $(FLAGS) s21_test.c
	gcc -o test_with_gcov s21_test.o gcov_s21_decimal.a $(LIBS) -lgcov
	./test_with_gcov
	
	lcov -t 'gcov_report' -o gcov_report.info -c -d .
	genhtml -o gcov_report gcov_report.info
	$(OPEN)

lint:
	cp ../materials/linters/.clang-format .
	clang-format -n *.c *.h
	clang-format -i *.c *.h
	clang-format -n *.c *.h
	rm -rf .clang-format

check: test lint
	cppcheck —enable=all —suppress=missingIncludeSystem *.c *.h
	valgrind --leak-check=full --show-leak-kinds=all -s ./test

clean: 
	rm -rf *.o
	rm -rf *.out
	rm -rf *.a
	rm -rf test test_with_gcov
	rm -rf *.gcno *.gcda *.info
	rm -rf gcov_report
