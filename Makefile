CC = gcc
CFLAGS = -g -Wall -Wextra

all: lorenz bifurcation

lorenz: lorenz.c draw.c

bifurcation: bifurcation.c draw.c

clean:
	rm -vf lorenz bifurcation *.pbm *.ppm

.PHONY: all test
