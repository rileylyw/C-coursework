#include "common.h"

bool strsimilar(const char* s1, const char* s2)
{
    char s1copy[100];
    char s2copy[100];
    int x=0;
    int y=0;
    for(unsigned int i=0; i<strlen(s1); i++){
        if((int)s1[i]>=65 && (int)s1[i]<=122){
            s1copy[x]=tolower(s1[i]);
            x++;
        }
    }
    for(unsigned int j=0; j<strlen(s2); j++){
        if((int)s2[j]>=65 && (int)s2[j]<=122){
            s2copy[y]=tolower(s2[j]);
            y++;
        }
    }
    s1copy[x]='\0';
    s2copy[y]='\0';

    // printf("s1 %s\n", s1copy);
    // printf("s2 %s\n", s2copy);

    if(strcmp(s1copy, s2copy)==0){
        return true;
    }
    return false;
}
