#include "specific.h"

void test(void){
    // int x=hash(50, "one");
    // printf("x: %d\n", x);
    // assert(hash(10, "hello") == 3);
    return;
}

unsigned long* _hashes(unsigned int sz, const char* s){
    unsigned long* hashes = ncalloc(KHASHES, sizeof(unsigned long));
    unsigned long bh = _hash(s);
    int len = strlen(s);
    srand(bh * (len*s[0] + s[len-1]));
    unsigned long h2 = bh;
    for (int i=0; i<KHASHES; i++){
        h2 = 33 * h2 ^ rand(); 
        hashes[i] = h2%sz;
    }
    return hashes;
}

int _hash(const char *s){
    unsigned long hash = 5381;
    int c;
    while((c = (*s++))){
        hash = 33 * hash ^ c;
    }
    return (int)hash;
}

dict* dict_init(unsigned int maxwords){
    test();
    dict* d = (dict*) ncalloc(1, sizeof(dict));
    d->size = maxwords * SCALE; //20times the size
    d->bitarray = (bool*) ncalloc(d->size, sizeof(bool));
    return d;
}

bool dict_add(dict* x,  const char* s){
    if(x == NULL){
        return false;
    }
    if(!dict_spelling(x, s)){
        unsigned long* hashes = _hashes(x->size, s);
        for(int i=0; i<KHASHES; i++){
            x->bitarray[hashes[i]] = true;
        }
        free(hashes);
        return true;
    }
    else{ //if existed also return true but no add
        return true;
    }
    return false;
}

bool dict_spelling(dict* x, const char* s){
    if(x == NULL){
        return false;
    }
    unsigned long* hashes = _hashes(x->size, s);
    for(int i=0; i<KHASHES; i++){
        if(!x->bitarray[hashes[i]]){
            free(hashes);
            return false;
        }
    }
    free(hashes);
    return true;
}

void dict_free(dict* x){
    free(x->bitarray);
    free(x);
}
