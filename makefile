CC = gcc
CFLAGS = -I include
SRC = $(wildcard src/*.c)
OUT = tracker

all: $(OUT)

tracker: src/main.c src/menu.c src/orders.c src/processing.c
	$(CC) $^ $(CFLAGS) -o tracker

tests: src/tests.c src/processing.c
	$(CC) src/tests.c src/processing.c $(CFLAGS) -o tests

clean:
	rm -f tracker tests
