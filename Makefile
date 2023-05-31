COLOR_RED=\033[1;31m
COLOR_BLUE=\033[1;34m
COLOR_GREEN=\033[1;32m
COLOR_YELLOW=\033[1;33m
COLOR_RESET=\033[0m

CC = gcc
CFLAGS = -Wall -Wextra -std=c2x -pedantic -g
COVERAGE_CFLAGS = -fprofile-arcs -ftest-coverage
COVERAGE_LFLAGS = -lgcov --coverage

BUILD_DIR = build
REPORT_DIR = report

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/main
	@echo "$(COLOR_GREEN)============ RUNNING VALGRIND ============$(COLOR_RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --quiet ./$<

$(BUILD_DIR)/main: $(OBJECTS)
	@echo "$(COLOR_YELLOW)============ GENERATING EXECUTABLE PROGRAM ============$(COLOR_RESET)"
	$(CC) $(COVERAGE_LFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@echo "$(COLOR_YELLOW)============ COMPILING FILES ============$(COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(COVERAGE_CFLAGS) -I. -c $< -o $@

.PHONY = clean

cov: $(BUILD_DIR)/main
	@echo "$(COLOR_GREEN)============ RUNNING MAIN PROGRAM ============$(COLOR_RESET)"
	@./$(BUILD_DIR)/main
	@gcov $(SOURCES) -o $(BUILD_DIR)
	@mv *.gcov $(BUILD_DIR)
	@lcov --capture --directory $(BUILD_DIR) --output-file coverage.info
	@genhtml coverage.info --output-directory $(REPORT_DIR)

clean:
	@echo "$(COLOR_YELLOW)============ CLEANING ============$(COLOR_RESET)"
	rm -rf $(BUILD_DIR) $(REPORT_DIR)
	rm -f coverage.info *.gcov
