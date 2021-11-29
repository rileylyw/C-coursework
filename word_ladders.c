/*  TBC  */
/* 34words.txt */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define SWAP(A,B) {char temp=*A; *A=*B; *B=temp;}
#define FILENAME "34words.txt"

// void permute(char* a, int s, int e);

int main(int argc, char* argv[]){
    FILE* fp=fopen(FILENAME, "r");

    char* s1, *s2;
    if(argc==0 || argc==1){
        fprintf(stderr, "please intput two words");
        exit(EXIT_FAILURE);
    }
    if(argc==3){
        s1=(char*)malloc(sizeof(char)*(strlen(argv[1])+1));
        strcpy(s1, argv[1]);
        s2=(char*)malloc(sizeof(char)*(strlen(argv[2])+1));
        strcpy(s2, argv[2]);
    }
    int len_s1=strlen(s1);
    int len_s2=strlen(s2);

    

    free(s1);
    free(s2);
    return 0;
}

void permute(char* a, int s, int e){
    if(s==e){
        // printf("%s\n", a);
        return a;
    }
    for(int i=s; i<=e; i++){
        SWAP((a+s), (a+i)); //bring one char to the front
        permute(a, s+1, e);
        SWAP((a+s), (a+i)); //backtrack
    }
}
