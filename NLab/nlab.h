#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "general.h"

#define BIGNUM 1000
#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20
#define strsame(A,B) (strcmp(A, B)==0)
#ifdef TEST
   #define ERROR(PHRASE) { return false; }
#else
   #define ERROR(PHRASE) { fprintf(stderr, \
            "Fatal Error %s occurred in %s, line %d\n", PHRASE, \
            __FILE__, __LINE__); \
            exit(EXIT_FAILURE); }
#endif

// #define CHARACTERS 26

struct var{
   char num[BIGNUM][BIGNUM];
   int height;
   int width;
};
typedef struct var var;

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE]; //instructions
   int cw; // Current Word
   var* variable;
};
typedef struct prog Program;

void readFile(char file[], Program* p);
bool verbose(char file[BIGNUM], int argc, char* argv[]);
bool Prog(Program *p);
bool Instrclist(Program *p);
bool Instrc(Program *p);
