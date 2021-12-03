#include "specific.h"
#include "boolarr.h"
#include <math.h>


boolarr* boolarr_init(void){
    boolarr* ba = (boolarr*)ncalloc(1, sizeof(boolarr));
    ba->size = 0;
    ba->capacity = BYTE;
    // ba->a = (byte*)ncalloc(1, 0);
    // ba->a[0].d = '1';
    return ba;
}

boolarr* boolarr_initstr(const char* str){
    boolarr* ba = boolarr_init();
    // if(ba->size >= ba->capacity){
    //     // ba->a = (bit*)nremalloc(ba->a, sizeof(bit)*ba->capacity*SCALEFACTOR);
    //     ba->a = (byte*)nrecalloc(ba->a, sizeof(byte)*ba->capacity, sizeof(byte)*ba->capacity*(int)(((int)strlen(str)+BYTE-1)/BYTE));
    //     // ba->a = (byte*)nrecalloc(ba->a, sizeof(byte)*ba->capacity, sizeof(byte)*ba->capacity*scale);
    //     // ba->capacity = ba->capacity*scale;
    //     ba->capacity = ba->capacity*(int)(((int)strlen(str)+BYTE-1)/BYTE);
    // }
    int len = strlen(str);
    
    for(int i=0, j=len-1; i<len; i++, j--){
        bool bit = str[i]=='1'?true:false;
        boolarr_set(ba, j, bit);
        // ba->a[ba->size] = (bool)(str[i]-'0'); 
        // ba->size = ba->size+1;
    }
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
    // ba->capacity = BYTE;
    // }
    // return ba->size;
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
    int scale = (int)ceil((double)n/BYTE);
    if(ba == NULL){
        return false;
    }
    if(ba->a == NULL){
        ba->a = (byte*)ncalloc(1, sizeof(byte));
    }
    if(scale >= ba->capacity){
        ba->a = (byte*)nrecalloc(ba->a, sizeof(byte)*ba->capacity, sizeof(byte)*ba->capacity*scale);
        ba->capacity = ba->capacity*scale;
    }
    if(b){
        ba->a[(int)n/BYTE] = ba->a[(int)n/BYTE] | (1<<(n-1)%BYTE);
    }
    else{
        ba->a[(int)n/BYTE] = ba->a[(int)n/BYTE] & ~(1<<(n-1)%BYTE);
    }
    // printf("a: %u\n", ba->a[0]);
    return true;
}

bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b){
    if(ba == NULL){
        return false;
    }
    
}

bool boolarr_issame(const boolarr* b1, const boolarr* b2){
    // if((b1 || b2) == NULL){
    //     return false;
    // }
}


bool boolarr_tostring(const boolarr* ba, char* str){
    if(ba == NULL){
        return false;
    }
}

bool boolarr_print(const boolarr* ba){
    
}



// bool boolarr_free(boolarr* p){
//    if(p==NULL){ 
//       return false;
//    }
//    free(p->a);
//    free(p);
//    return true;
// }