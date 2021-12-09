#include "specific.h"

unsigned long* _hashes(unsigned int sz, const char* s){
    unsigned long* hashes = (unsigned long*)ncalloc(KHASHES, sizeof(unsigned long));
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

void test(void){
    int x = _hash("hello");
    int y = _hash("testing");
    assert(x != y);
    unsigned long* hv1=_hashes(50, "sornari");
    for(int i=0; i< KHASHES; i++){ //test hash values within hash table size range
        assert(hv1[i]>0);
        assert(hv1[i]<50);
        // printf("%d, %lu\n", i, hv[i]);
    }
    free(hv1);
    /* init dict */
    dict* d = (dict*) ncalloc(1, sizeof(dict));
    d->size = 50 * SCALE; //20times the size
    d->bitarray = (bool*) ncalloc(d->size, sizeof(bool));
    assert(!dict_add(NULL, "sornari"));
    assert(dict_add(d, "sornari"));
    assert(dict_add(d, "sornari"));
    assert(dict_add(d, "letterers"));
    unsigned long* hv2=_hashes(50*20, "letterers");
    for(int i=0; i<KHASHES; i++){ //test if respective bits are set to true
        assert(d->bitarray[hv2[i]] == true);
        assert(d->bitarray[777] == false);
        assert(d->bitarray[123] == false);
        assert(d->bitarray[999] == false);
        // assert(d->bitarray[10000] == NULL);
        // printf("boolarray %s\n", d->bitarray[hv2[i]]?"true":"false");
    }
    // printf("1000 %lu\n", hv2[1000]);
    // for(int i=0; i<d->size+20; i++){
    //     printf("%d, %s\n", i, d->bitarray[i]?"true":"false");
    // }
    free(hv2);
    assert(dict_add(d, "letterers"));
    assert(dict_add(d, "interconnect"));
    assert(dict_add(d, "interconnect"));
    assert(dict_add(d, "hello"));
    assert(dict_add(d, "-!?"));
    assert(!dict_spelling(NULL, "sornari"));
    assert(dict_spelling(d, "sornari"));
    assert(dict_spelling(d, "letterers"));
    assert(dict_spelling(d, "interconnect"));
    assert(dict_spelling(d, "hello"));
    assert(dict_spelling(d, "-!?"));
    assert(!dict_spelling(d, "sornarii"));
    assert(!dict_spelling(d, "123"));
    dict_free(d);
    return;
}
