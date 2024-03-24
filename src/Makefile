CC = gcc
CFLAGS = -I./brick_game/tetris -I./gui/cli -Wall -Wextra -Werror -std=c11 -Wpedantic
LDFLAGS=-L./brick_game/tetris -ltetris_game -lncurses
LIB_FLAGS = $(shell pkg-config check ncurses --cflags --libs)
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

OS = $(shell uname -s)

ifeq ($(OS), Darwin)
	TEST_LIBS = -lcheck -lm -lpthread
else
	TEST_LIBS = -lcheck -lsubunit -pthread -lrt -lm
endif

PROJECT = tetris_cli

all : 


install : tetris.a
	# $(CC) $(CFLAGS) tetris_cli.c gui/cli/cli.c tetris.a -o tetris_cli $(LIB_FLAGS)
	$(CC) $(CFLAGS) tetris_cli.c gui/cli/cli.c tetris.a -o tetris_cli $(LIB_FLAGS) -fsanitize=address

uninstall :


rebuild : clean install


clean :
	rm -rf gui.a tetris.a tetris_cli

dvi:  
ifeq ($(OS), Darwin)
#	open ./documentation.md
else
#	cat ./documentation.md
endif


dist :


test :

leaks :
	leaks -atExit -- ./tetris_game

gcov_report : test
	rm -rf report
	mkdir report
	gcovr -r . --html --html-details -o report/gcov_report.html --exclude tests/
	open report/gcov_report.html

tetris.a :
	$(CC) $(CFLAGS) -c brick_game/tetris/tetris.c $(LIB_FLAGS)
	ar rc tetris.a tetris.o
	ranlib tetris.a
	rm -rf *.o

format :
	clang-format --style=file:"../materials/linters/.clang-format" -i *.c *.h */*/*.c */*/*.h

check :
	clang-format --style=file:"../materials/linters/.clang-format" -n *.c *.h */*/*.c */*/*.h