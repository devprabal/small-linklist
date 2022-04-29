COLOR_RED=\033[1;31m
COLOR_BLUE=\033[1;34m
COLOR_GREEN=\033[1;32m
COLOR_YELLOW=\033[1;33m
COLOR_RESET=\033[0m

CC=gcc
CFLAGS=-Wall

OBJS+=node/src/node.o
OBJS+=main/src/main.o

.PHONY= all clean

all: main.out
	@echo -e "$(COLOR_GREEN)============ RUNNING MAIN PROGRAM ============$(COLOR_RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --quiet ./$<

%.o: %.c
	@echo -e "$(COLOR_YELLOW)============ COMPILING FILES ============$(COLOR_RESET)"
	$(CC) $(CFLAGS) -I. -c $< -o $@

main.out: $(OBJS)
	@echo -e "$(COLOR_YELLOW)============ GENERATING EXECUTABLE PROGRAM ============$(COLOR_RESET)"
	$(CC) $^ -o $@

clean:
	@echo -e "$(COLOR_YELLOW)============ CLEANING ============$(COLOR_RESET)"
	rm -f main.out
	rm -f $(OBJS)
