CC = gcc
CFLAGS = -I include
SRC = $(wildcard src/*.c)
OUT = t

all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)
