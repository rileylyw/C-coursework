#include "specific.h"
#include "boolarr.h"


boolarr* boolarr_init(void){
    boolarr* ba = (boolarr*)ncalloc(1, sizeof(boolarr));
    ba->size = 0;
    ba->capacity = INITSIZE;
    ba->a = (boolean*)ncalloc(ba->capacity, sizeof(boolean));
    // ba->a[0].d = '1';
    return ba;
}

boolarr* boolarr_initstr(const char* str){
    boolarr* ba=boolarr_init();
    if(ba->size >= ba->capacity){
        ba->a = (boolean*)nremalloc(ba->a, sizeof(boolean)*ba->capacity*SCALEFACTOR);
        ba->capacity = ba->capacity*SCALEFACTOR;
    } 
    for(unsigned int i=0; i<strlen(str); i++){
        ba->a[ba->size].d = str[i]-'0'; //char to int
        ba->size = ba->size+1;
    }
    return ba;
}

boolarr* boolarr_clone(const boolarr* ba){
    boolarr* ba_copy = boolarr_init();
    for(int i=0; i<ba->size; i++){
        ba_copy->a[i].d=ba->a[i].d;
        ba_copy->a[i].valid_bit=ba->a[i].valid_bit;
    }
    ba_copy->capacity = ba->capacity;
    ba_copy->size = ba->size;
    return ba_copy;
}

unsigned int boolarr_size(const boolarr* ba){
    int* array=(int*)malloc(sizeof(int)*ba->size);
    
    free(array);
}



// bool boolarr_free(boolarr* p){
//    if(p==NULL){ 
//       return false;
//    }
//    free(p->a);
//    free(p);
//    return true;
// }