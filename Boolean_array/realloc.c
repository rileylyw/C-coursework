#include "specific.h"
#include "boolarr.h"


boolarr* boolarr_init(void){
    boolarr* ba = (boolarr*)ncalloc(1, sizeof(boolarr));
    ba->size = 0;
    ba->capacity = BYTE;
    ba->a = (bit*)ncalloc(ba->capacity, sizeof(bit));
    // ba->a[0].d = '1';
    return ba;
}

boolarr* boolarr_initstr(const char* str){
    boolarr* ba=boolarr_init();
    if(ba->size >= ba->capacity){
        ba->a = (bit*)nremalloc(ba->a, sizeof(bit)*ba->capacity*SCALEFACTOR);
        ba->capacity = ba->capacity*SCALEFACTOR;
    }
    int len = strlen(str);
    

    // for(unsigned int i=0; i<strlen(str); i++){
    //     ba->a[ba->size] = (bool)(str[i]-'0'); //char to int
    //     ba->size = ba->size+1;
    // }
    return ba;
}

boolarr* boolarr_clone(const boolarr* ba){
    boolarr* ba_copy = boolarr_init();
    for(int i=0; i<ba->size; i++){
        ba_copy->a[i] = ba->a[i];
    }
    ba_copy->capacity = ba->capacity;
    ba_copy->size = ba->size;
    return ba_copy;
}

unsigned int boolarr_size(const boolarr* ba){
    if(ba==NULL){
        return 0;
    }
    return ba->size;
}

unsigned int boolarr_count1s(const boolarr* ba){
    if(ba==NULL){
        return 0;
    }
    int count=0;
    for(int i=0; i<ba->size; i++){
        if(ba->a[i]==true){
            count++;
        }
    }
    return count;
}

bool boolarr_set(boolarr* ba, const unsigned int n, const bool b){
    ba->a[n] = b;
    return ba->a[n];
}

// bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b){
    
// }






// bool boolarr_free(boolarr* p){
//    if(p==NULL){ 
//       return false;
//    }
//    free(p->a);
//    free(p);
//    return true;
// }