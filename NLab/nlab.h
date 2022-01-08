#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "general.h"
// #include "stack.h"

// #define FORMATSTR "%i"
// #define ELEMSIZE 20
// #define STACKTYPE "Linked"
#define BIGNUM 1000
#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20
#define CHARACTER 26
#define SCALEFACTOR 2
#define FIXEDSIZE 100
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
   // int size; //size of respective stack
};
typedef struct var var;

struct stack {
   /* Underlying array */
   var* a;
   int size;
   int capacity;
};
typedef struct stack stack;

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE]; //instructions
   int cw; // Current Word
   var variable[CHARACTER];
   stack* stack;
};
typedef struct prog Program;

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
// void AssignValues(Program *p, int h, int w, int pos, int value);
void AllocSpace(Program *p, int h, int w, int pos);
stack* stack_init(void);
void stack_push(stack* s, var d);
bool stack_free(stack* s);
bool stack_pop(stack* s, var* d);
var* MakeIntMatrix(Program *p, int pos, int value);
void FreeIntMatrix(var* temp);
