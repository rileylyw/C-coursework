#include "specific.h"

void test(void){
    int x=hash(50, "one");
    printf("x: %d\n", x);
    assert(hash(10, "hello") == 3);
    return;
}

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
        if(temp->word != s){ //if repeated dont store
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


// void print(dict* x){
//     int i;
//     for(i = 0; i < x->size; i++){
//         node* temp = x->hash[i];
//         printf("x->hash[%d]-->", i);
//         while(temp)        {
//             printf("%s -->",temp->word);
//             temp = temp->next;
//         }
//         printf("NULL\n");
//     }
// }
