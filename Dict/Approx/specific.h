#include "../dict.h"

#define KHASHES 11
#define SCALE 20 

struct dict{
    int size;
    bool* bitarray;
};

unsigned long* _hashes(unsigned int sz, const char* s);
int _hash(const char *s);
