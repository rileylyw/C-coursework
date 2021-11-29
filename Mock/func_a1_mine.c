#include "common.h"

bool struses(const char* s1, const char* s2)
{
    unsigned int count=0;

    for(unsigned int j=0; j<strlen(s1); j++){
        for(unsigned int i=0; i<strlen(s2); i++){
            if(s1[j]==s2[i]){
                count++;
            }
        }
    }
    if(strlen(s1)==count){
        return true;
    }
    return false;
}

