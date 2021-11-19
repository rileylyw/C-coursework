CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99 -Wvla -Wfloat-equal -O1 -Werror
SANITIZE=-fsanitize=undefined -fsanitize=address -g3
LDLIBS= -lm

rollerboard : rollerboard.c
	$(CC) rollerboard.c -o rollerboard $(CFLAGS) $(LDLIBS) $(SANITIZE)
