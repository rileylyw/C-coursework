#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "general.c"

#define BIGNUM 1000
#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20
#define strsame(A,B) (strcmp(A, B)==0)
#define ERROR(PHRASE) { fprintf(stderr, \
          "Fatal Error %s occurred in %s, line %d\n", PHRASE, \
          __FILE__, __LINE__); \
          exit(EXIT_FAILURE); }
#define CHARACTERS 26
// #define DINTERP
// #ifdef DINTERP
// #endif

struct var{
//    char var[CHARACTERS]; //A-Z
   char num[BIGNUM][BIGNUM];
   int height;
   int width;
};
typedef struct var var;

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE];
   int cw; // Current Word
   var* variable;
};
typedef struct prog Program;

void Prog(Program *p);
void Instrclist(Program *p);
void test(void);
// void Code(Program *p);
// void Statement(Program *p);



void test(void){
    return;
}
