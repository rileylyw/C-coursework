#include "specific.h"

int hash(unsigned int sz, const char *s){
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
    d->size = maxwords * SCALE; //twice the size
    d->hash = (node**) ncalloc(d->size, sizeof(node*));
    return d;
}

bool dict_add(dict* x,  const char* s){
    if(x == NULL){
        return false;
    }
    int hashValue = hash(x->size, s);
    if(x->hash[hashValue] == NULL){
        x->hash[hashValue] = allocateData(s);
        return true;
    }
    else{
        node* temp = x->hash[hashValue];
        if(!dict_spelling(x, s)){ //if repeated dont store
            while(temp->next){
                temp = temp->next;
            }
            temp->next = allocateData(s);
        }
        return true;
    }
    return false;
}

node* allocateData(const char *s){
    node* p;
    p = (node*) ncalloc(1, sizeof(node));
    p->word = (char*) ncalloc(strlen(s)+1, sizeof(char));
    strcpy(p->word, s);
    p->next = NULL;
    return p;
}

bool dict_spelling(dict* x, const char* s){
    if(x == NULL){
        return false;
    }
    int hashValue = hash(x->size, s);
    node* temp = x->hash[hashValue];
    while(temp){
        if(strcmp(temp->word, s) == 0){
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void dict_free(dict* x){
    node* temp;
    for(int i=0; i<x->size; i++){
        while(x->hash[i] != NULL){
            temp = x->hash[i];
            x->hash[i] = x->hash[i]->next;
            free(temp->word);
            free(temp);
        }
    }
    free(x->hash);
    free(x);
}

void test(void){
    assert(hash(10, "hello") == 3);
    assert(hash(1000, "testing") == 19);
    assert(hash(1000, "testing") < 1000);
    assert(hash(50, "oneoneone") == 13);
    assert(hash(50, "oneoneone") < 50);
    /* init dict */
    dict* d = (dict*) ncalloc(1, sizeof(dict));
    d->size = 50 * SCALE;
    d->hash = (node**) ncalloc(d->size, sizeof(node*));
    assert(!dict_add(NULL, "sornari"));
    assert(dict_add(d, "sornari"));
    assert(dict_add(d, "sornari"));
    assert(dict_add(d, "letterers"));
    assert(dict_add(d, "letterers"));
    assert(dict_add(d, "interconnect"));
    assert(dict_add(d, "interconnect"));
    assert(dict_add(d, "hello"));
    /* sornari, letterers, interconnect share the same hash value */
    int hv1 = hash(d->size, "sornari");
    int hv2 = hash(d->size, "letterers");
    int hv3 = hash(d->size, "interconnect");
    assert(strcmp(d->hash[hv1]->word, "sornari") == 0); //first node
    assert(strcmp(d->hash[hv2]->next->word, "letterers") == 0); //second node
    assert(strcmp(d->hash[hv3]->next->next->word, "interconnect") == 0); //third node
    assert(d->hash[hv1]->next->next->next == NULL); //if word repeated, didnt add to dict
    assert(!dict_spelling(NULL, "sornari"));
    assert(dict_spelling(d, "sornari"));
    assert(dict_spelling(d, "letterers"));
    assert(dict_spelling(d, "interconnect"));
    assert(dict_spelling(d, "hello"));
    assert(!dict_spelling(d, "sornarii"));
    assert(!dict_spelling(d, "123"));
    node* test = allocateData("123");
    assert(strcmp(test->word, "123") == 0);
    assert(strcmp(test->word, "abc") != 0);
    assert(test->next == NULL);
    free(test->word);
    free(test);
    dict_free(d);
    return;
}
