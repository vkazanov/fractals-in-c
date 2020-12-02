CC = gcc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm

all: lorenz bifurcation koch peano

lorenz: lorenz.c draw.c

bifurcation: bifurcation.c draw.c

koch: koch.c draw.c

peano: peano.c draw.c

clean:
	rm -vf lorenz bifurcation koch peano *.pbm *.ppm

.PHONY: all test
