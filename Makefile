CC = gcc
CFLAGS = -g -Wall -Wextra
LOADLIBES=-lm

EXECUTABLES=lorenz bifurcation koch peano hilbert

.PHONY: all
all: $(EXECUTABLES)

$(EXECUTABLES): draw.o

draw.o: draw.h

.PHONY: clean
clean:
	rm -vf *.o $(EXECUTABLES) *.pbm *.ppm
