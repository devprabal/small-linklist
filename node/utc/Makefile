ROOT_DIR=../..

COLOR_RED=\033[1;31m
COLOR_BLUE=\033[1;34m
COLOR_GREEN=\033[1;32m
COLOR_YELLOW=\033[1;33m
COLOR_RESET=\033[0m

CC=gcc
CFLAGS=-Wall -fprofile-arcs -ftest-coverage
CHECK_FRMWRK_FLAGS=-I/usr/include -L/usr/lib -L/usr/lib/x86_64-linux-gnu -lcheck -lcheck_pic

SRC_DIR=$(ROOT_DIR)/node/src
TRG+=node.c

.PHONY=clean test gcov_files

test: utc.out
	@echo -e "$(COLOR_GREEN)============ RUNNING TESTS FOR $(TRG) ============$(COLOR_RESET)"
	valgrind --leak-check=full --quiet ./$<

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(ROOT_DIR) -c $< -o $@

utc.o: utc.c
	$(CC) $(CFLAGS) -I$(ROOT_DIR) -c $< $(CHECK_FRMWRK_FLAGS) -o $@

utc.out: utc.o $(patsubst %.c,%.o,$(TRG))
	$(CC) $(CFLAGS) $(CHECK_FRMWRK_FLAGS) $^ -o $@

gcov_files:
	gcov --object-directory . $(TRG) utc.c

clean:
	rm -f *.o *.gcno *.out *.gcda *.gcov
