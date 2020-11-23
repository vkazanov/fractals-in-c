CC = gcc
CFLAGS = -g -Wall -Wextra

all: lorenz

lorenz: lorenz.c draw.c

clean:
	rm -vf lorenz *.pbm

.PHONY: all test
