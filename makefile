CC = gcc
CFLAGS = -I include
SRC = $(wildcard src/*.c)
OUT = tracker

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)
