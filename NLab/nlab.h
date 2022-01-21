#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "general.h"

#define BIGNUM 100
#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20
#define CHARACTER 26
#define SCALEFACTOR 2
#define FIXEDSIZE 10
#define strsame(A,B) (strcmp(A, B)==0)
#define CW (int)(p->wds[p->cw][1])-'A'
#define CW_minus2 (int)(p->wds[p->cw-2][1])-'A'
#define CW_minus1 (int)(p->wds[p->cw-1][1])-'A'
#define CW_plus2 (int)(p->wds[p->cw+2][1])-'A'
#define CW_plus1 (int)(p->wds[p->cw+1][1])-'A'
#define _1wordback p->cw-1
#define _2wordsback p->cw-2
#define _1wordforward p->cw+1
#define _2wordsforward p->cw+2

#ifdef TEST
   #define ERROR(PHRASE) { return false; }
#else
   #define ERROR(PHRASE) { fprintf(stderr, \
            "Error: %s, occurred in line %d\n", PHRASE, \
             __LINE__); \
            exit(EXIT_FAILURE); }
#endif


struct var{
   int** num;
   int height;
   int width;
};
typedef struct var var;

struct stack {
   var* a;
   int size;
   int capacity;
};
typedef struct stack stack;

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE]; //instructions
   int cw; // Current Word
   var variable[CHARACTER];
   int pos;
   int workingpos;
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
#ifdef INTERP
void AssignValues(Program *p, int pos, int value);
void AllocSpace(Program *p, int h, int w, int pos);
stack* stack_init(void);
void stack_push(stack* s, var* d);
bool stack_free(stack* s);
bool stack_pop(stack* s, var* temp);
var MakeIntMatrix(int num);
void FreeNum(var temp1, var temp2, var temp3);
void StackToVar(Program *p);
bool Pushable(Program *p);
bool Pushable_VarFirst(Program *p);
bool Pushable_IntFirst(Program *p);
void PrintVar(Program *p, int pos);
void ReadArray(char string2[], Program *p);
var TempVarWithBounds(var tempvar1);
var PadZeros(var tempvar1);
void CountTrueValues(var tempvar_bound, var tempvar1);
void PushPrevItem(Program *p);
var TempVarForResult(var tempvar2);
void PushResult(Program *p, var tempvar1, var tempvar2, var tempvar3);
void U_NOT(Program *p);
void U_EIGHTCOUNT(Program *p);
void B_AND(Program *p, var tempvar1, var tempvar2, var tempvar3);
void B_OR(Program *p, var tempvar1, var tempvar2, var tempvar3);
void B_GREATER(Program *p, var tempvar1, var tempvar2, var tempvar3);
void B_LESS(Program *p, var tempvar1, var tempvar2, var tempvar3);
void B_ADD(Program *p, var tempvar1, var tempvar2, var tempvar3);
void B_TIMES(Program *p, var tempvar1, var tempvar2, var tempvar3);
void B_EQUALS(Program *p, var tempvar1, var tempvar2, var tempvar3);
#endif
