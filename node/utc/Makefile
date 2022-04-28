ROOT_DIR=../..

CC=gcc
CFLAGS=-Wall -ftest-coverage
CHECK_FRMWRK_FLAGS=-I/usr/include -L/usr/lib -lcheck

TRG_SRC+=node/src/node.c

.PHONY=test

test: utc.c
	$(CC) $(CFLAGS) -I$(ROOT_DIR) $(ROOT_DIR)/node/src/node.c -c -o node.o
	$(CC) $(CFLAGS) -I$(ROOT_DIR) $^ $(CHECK_FRMWRK_FLAGS) -c -o utc.o
	gcc -L/usr/lib -lcheck utc.o node.o -o utc.out
	./utc.out
	gcov $(ROOT_DIR)/$(TRG_SRC)