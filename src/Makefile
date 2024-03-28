CC := gcc
CFLAGS := -I./brick_game/tetris -I./gui/cli -Wall -Wextra -Werror -std=c11 -Wpedantic
LDFLAGS := -L./brick_game/tetris -ltetris_game -lncurses
LIB_FLAGS := $(shell pkg-config check ncurses --static --cflags --libs)
GCOV_FLAGS := -fprofile-arcs -ftest-coverage

OS := $(shell uname -s)

PROJECT := tetris_cli
GUI := gui/cli/cli.c

all : 


install : tetris.a
	$(CC) $(CFLAGS) tetris_cli.c $(GUI) tetris.a -o tetris_cli $(LIB_FLAGS)

install-sanitised : tetris.a
	$(CC) $(CFLAGS) tetris_cli.c $(GUI) tetris.a -o tetris_cli $(LIB_FLAGS) -fsanitize=address

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
	$(CC) $(CFLAGS) tests/tetris_cli_test.c brick_game/tetris/tetris_test.c tetris.a -o tetris_test $(LIB_FLAGS)
	./tetris_test

leaks :
	leaks -atExit -- ./tetris_cli

gcov_report :
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c brick_game/tetris/tetris.c $(LIB_FLAGS)
	ar rc tetris.a tetris.o
	ranlib tetris.a
	rm -rf *.o
	$(CC) $(CFLAGS) tests/tetris_cli_test.c brick_game/tetris/tetris_test.c tetris.a -o tetris_test $(LIB_FLAGS) -lgcov
	./tetris_test
	rm -rf report
	mkdir report
	gcovr --html-details report/coverage.html
	rm -rf *.gcda *.gcno *.o
	open report/coverage.html

tetris.a :
	$(CC) $(CFLAGS) -c brick_game/tetris/tetris.c $(LIB_FLAGS)
	ar rc tetris.a tetris.o
	ranlib tetris.a
	rm -rf *.o

format :
	clang-format --style=file:"../materials/linters/.clang-format" -i *.c *.h */*/*.c */*/*.h

check :
	clang-format --style=file:"../materials/linters/.clang-format" -n *.c *.h */*/*.c */*/*.h