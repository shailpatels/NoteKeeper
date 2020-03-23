.DEFAULT_GOAL := build
CC=cc

build:
	$(CC) main.c -o note -O3

clean:
	rm note