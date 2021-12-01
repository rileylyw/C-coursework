#include <stdbool.h>

typedef unsigned char bit;

#define BYTE 8
#define SCALEFACTOR 2

// typedef struct boolean{
//     bit d;
//     bool valid_bit;
// } boolean;

typedef struct boolarr{
    bit* a;
    int size; //valid bit
    int capacity;
} boolarr;