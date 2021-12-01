#include <stdbool.h>

typedef unsigned int bit;

#define INITSIZE 100
#define SCALEFACTOR 2

typedef struct boolean{
    bit d;
    bool valid_bit;
} boolean;

typedef struct boolarr{
    boolean* a;
    int size;
    int capacity;
} boolarr;