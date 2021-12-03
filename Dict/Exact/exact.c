#include "specific.h"

void test(void){
    return;
}

int hash(unsigned int sz, char *s){
    unsigned long hash = 5381;
    int c;
    while((c = (*s++))){
        hash = 33 * hash ^ c;
    }
    return (int)(hash%sz);
}

dict* dict_init(unsigned int maxwords){
    test();
    dict* d = (dict*) ncalloc(1, sizeof(dict));
    d->hash = (data*) ncalloc(d->size, sizeof(data));
    return d;
}

bool dict_add(dict* x,  const char* s){
    int hashValue = hash(x->size, s);
    if(x->hash->word == NULL){
        x->hash->word = (char*) ncalloc(strlen(s)+1, sizeof(char));
    }
    if(x == NULL){
        return false;
    }
    if(x->hash->word == NULL){
        x->hash[hashValue].next = allocateData(s);
        return true;
    }
}

data* allocateData(char *s){
    data* p;
    p = (data*) ncalloc(1, sizeof(data));
    strcpy(p->word, s);
    p->next = NULL;
    return p;
}
