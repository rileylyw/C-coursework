#include "../dict.h"

#define SCALE 2

typedef struct node{
    char* word;
    struct node* next;
} node;

struct dict{
    node** hash;
    int size;
};

int hash(unsigned int sz, const char *s);
node* allocateData(const char *s);
