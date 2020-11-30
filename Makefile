CC = gcc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm

all: lorenz bifurcation koch

lorenz: lorenz.c draw.c

bifurcation: bifurcation.c draw.c

koch: koch.c draw.c

clean:
	rm -vf lorenz bifurcation koch *.pbm *.ppm

.PHONY: all test
