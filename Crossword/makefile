CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99 -Wvla -Wfloat-equal -O1 -Werror -g3 #-fsanitize=undefined -fsanitize=address
LDLIBS= -lm

crossword : crossword.c cwddriver.c crossword.h
	$(CC) crossword.c cwddriver.c -o crossword $(CFLAGS) $(LDLIBS)
