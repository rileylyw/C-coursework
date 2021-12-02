#include <math.h>
#include <stdbool.h>

typedef unsigned char byte;

#define BYTE 8
#define SCALEFACTOR 2

// typedef struct boolean{
//     bit d;
//     bool valid_bit;
// } boolean;

typedef struct boolarr{
    byte* a;
    int size; //valid bit
    int capacity;
} boolarr;