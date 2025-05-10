COLOR_RED=\033[1;31m
COLOR_BLUE=\033[1;34m
COLOR_GREEN=\033[1;32m
COLOR_YELLOW=\033[1;33mb
COLOR_RESET=\033[0m

IS_MAC = $([[ $(uname -a | cut -d' ' -f1) == "Darwin" ]])

ifeq ($(IS_MAC), 0) ## yes mac
CC = clang
COVERAGE_CFLAGS = -fprofile-instr-generate -fcoverage-mapping
else ## assuming Linux
CC = gcc
COVERAGE_CFLAGS = -fprofile-arcs -ftest-coverage -fprofile-abs-path
endif

CFLAGS = -Wall -Wextra -std=c11 -pedantic -g  -D_XOPEN_SOURCE=600
##TODO: use only c11 without gnu extensions or posix std (feature_test_macros(7))
COVERAGE_LFLAGS = --coverage

# COVERAGE_CFLAGS = -fprofile-arcs -ftest-coverage -fprofile-abs-path
# COVERAGE_CFLAGS = -fprofile-instr-generate -fcoverage-mapping


BUILD_DIR = build
REPORT_DIR = report

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

MAKE_SUPPRESS_ENTER_LEAVE_DIR_MSG = --no-print-directory

all:
	@make check_dependencies $(MAKE_SUPPRESS_ENTER_LEAVE_DIR_MSG)
	@make valgrind $(MAKE_SUPPRESS_ENTER_LEAVE_DIR_MSG)

check_dependencies:
## This recipe will only work on ubuntu because of `apt` in the instructions
	@echo "$(COLOR_YELLOW)============ CHECKING DEPENDENCIES ============$(COLOR_RESET)"
	@command -v valgrind > /dev/null || (echo "$(COLOR_RED)Valgrind is not installed. Installing...$(COLOR_RESET)" && sudo apt install -y valgrind)
## Not using libcheck anymore
## On Arch, libcheck is installed elsewhere
# @ls /usr/lib/x86_64-linux-gnu/ | grep libcheck.a > /dev/null || (echo "$(COLOR_RED)Check is not installed. Installing...$(COLOR_RESET)" && sudo apt install -y check)
	@command -v lcov > /dev/null || (echo "$(COLOR_RED)Lcov is not installed. Installing...$(COLOR_RESET)" && sudo apt install -y lcov)
	@command -v gcovr > /dev/null || (echo "$(COLOR_RED)Gcovr is not installed. Installing...$(COLOR_RESET)" && sudo apt install -y gcovr)

$(BUILD_DIR)/main: $(OBJECTS)
	@echo "$(COLOR_YELLOW)============ GENERATING EXECUTABLE PROGRAM ============$(COLOR_RESET)"
	$(CC) $(COVERAGE_LFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@echo "$(COLOR_YELLOW)============ COMPILING FILES ============$(COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(COVERAGE_CFLAGS) -I. -c $< -o $@

.PHONY: clean

cov: $(BUILD_DIR)/main
## Generates line coverage report in `report/index.html`
	@echo "$(COLOR_GREEN)============ RUNNING MAIN PROGRAM ============$(COLOR_RESET)"
	@./$(BUILD_DIR)/main
	@gcov --preserve-paths $(SOURCES) -o $(BUILD_DIR)
	@mv *.gcov $(BUILD_DIR)
	@mkdir -p $(REPORT_DIR)
	@lcov --capture --directory $(BUILD_DIR) --output-file $(REPORT_DIR)/coverage.info
	@genhtml $(REPORT_DIR)/coverage.info --output-directory $(REPORT_DIR)

clean:
	@echo "$(COLOR_YELLOW)============ CLEANING ============$(COLOR_RESET)"
	rm -rf $(BUILD_DIR) $(REPORT_DIR)

valgrind: $(BUILD_DIR)/main
## Does not install dependencies
	@echo "$(COLOR_GREEN)============ RUNNING VALGRIND ============$(COLOR_RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --quiet ./$(BUILD_DIR)/main

sonarqube:
## This recipe is only for github actions (on push/pull) (not local)
## Does not check for dependencies
## Dependencies are installed by github actions file .github/workflows/build.yml
	@make clean $(MAKE_SUPPRESS_ENTER_LEAVE_DIR_MSG)
	@make valgrind $(MAKE_SUPPRESS_ENTER_LEAVE_DIR_MSG)
	@make cov $(MAKE_SUPPRESS_ENTER_LEAVE_DIR_MSG)

mac: $(BUILD_DIR)/main
## Does not install dependencies
	@echo "$(COLOR_GREEN)============ RUNNING LEAKS ============$(COLOR_RESET)"
	leaks --atExit -- ./$(BUILD_DIR)/main
