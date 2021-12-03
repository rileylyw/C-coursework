#include "../dict.h"

typedef struct data{
    bool occupied;
    char* word;
    struct data* next;
} data;

typedef struct dict{
    data* hash;
    int size;
} dict;

int hash(unsigned int sz, char *s);