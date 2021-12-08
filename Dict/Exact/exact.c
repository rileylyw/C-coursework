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
        node* temp = x->hash[hashValue]; //TODO: repeated chain
        if(strcmp(temp->word, s)){ //if repeated dont store
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
        if(strcmp(temp->word, s)==0){
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


void print(dict* x){
    int i;
    for(i = 0; i < x->size; i++){
        node* temp = x->hash[i];
        printf("x->hash[%d]-->", i);
        while(temp)        {
            printf("%s -->",temp->word);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

void test(void){
    // int x=hash(100, "sdgd");
    // printf("x: %d\n", x);
    assert(hash(10, "hello") == 3);
    assert(hash(1000, "testing") == 19);
    assert(hash(1000, "testing") < 1000);
    assert(hash(50, "oneoneone") == 13);
    assert(hash(50, "oneoneone") < 50);
    /* init dict */
    dict* d = (dict*) ncalloc(1, sizeof(dict));
    d->size = 50 * SCALE;
    d->hash = (node**) ncalloc(d->size, sizeof(node*));
    assert(!dict_add(NULL, "cainitic"));
    assert(dict_add(d, "cainitic"));
    assert(dict_add(d, "cainitic"));
    assert(dict_add(d, "bournes"));
    assert(dict_add(d, "bournes"));
    print(d);
    int hv1 = hash(d->size, "cainitic");
    int hv2 = hash(d->size, "bournes"); //cainitic & bournes share the same hash value
    assert(strcmp(d->hash[hv1]->word, "cainitic") == 0); //first node
    assert(strcmp(d->hash[hv2]->next->word, "bournes") == 0); //second node
    assert(d->hash[hv1]->next->next == NULL); //if word repeated, didnt add to dict
    node* test = allocateData("123");
    assert(strcmp(test->word, "123") == 0);
    assert(strcmp(test->word, "abc") != 0);
    assert(test->next == NULL);
    free(test->word);
    free(test);



    // print(d);
    dict_free(d);
    return;
}
