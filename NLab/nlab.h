#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "general.h"
#include "stack.h"

#define FORMATSTR "%i"
#define ELEMSIZE 20
#define STACKTYPE "Linked"
#define BIGNUM 1000
#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20
#define CHARACTER 26
#define strsame(A,B) (strcmp(A, B)==0)
#ifdef TEST
   #define ERROR(PHRASE) { return false; }
#else
   #define ERROR(PHRASE) { fprintf(stderr, \
            "Fatal Error %s occurred in %s, line %d\n", PHRASE, \
            __FILE__, __LINE__); \
            exit(EXIT_FAILURE); }
#endif

struct var{
   int** num; //n2dcalloc
   int height;
   int width;
};
typedef struct var var;

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE]; //instructions
   int cw; // Current Word
   var* variable[CHARACTER];
};
typedef struct prog Program;

// struct dataframe {
//    stacktype i; //value
//    struct dataframe* next;
// };
// typedef struct dataframe dataframe;

// struct stack {
//    /* Underlying array */
//    dataframe* start;
//    int size;
// };
// typedef struct stack stack;

void readFile(char file[], Program* p);
bool verbose(char file[BIGNUM], int argc, char* argv[]);
bool Prog(Program *p);
bool InstrcList(Program *p);
bool Instrc(Program *p);
bool Print(Program *p);
bool Set(Program *p);
bool PolishList(Program *p);
bool Polish(Program *p);
bool Varname(Program *p);
bool String(Program *p);
bool PushDown(Program *p);
bool Integer(Program *p);
bool UnaryOp(Program *p);
bool BinaryOp(Program *p);
bool Create(Program *p);
bool Row(Program *p);
bool Col(Program *p);
bool FileName(Program *p);
bool Loop(Program *p);
void AssignValues(Program *p, int pos, int value);
void AllocSpace(Program *p, int h, int w, int pos);
