#include <stdio.h>
#include <string.h>

#define SWAP(A,B) {char temp=*A; *A=*B; *B=temp;}

void permute(char* a, int s, int e);

int main(void){
    char str[]="ABC";
    int n=strlen(str);
    permute(str, 0, n-1);
    return 0;
}

void permute(char* a, int s, int e){
    if (s==e){
        printf("0 %s\n", a);
        return;
    }
    for (int i=s; i<=e; i++){
        // printf("1 %s i:%i s:%i e:%i\n", a, i, s, e);
        SWAP((a+s), (a+i));
        printf("2 %s i:%i s:%i e:%i\n", a, i, s, e);
        
        permute(a, s+1, e);
        SWAP((a+s), (a+i));
        printf("3 %s i:%i s:%i e:%i\n", a, i, s, e);
    }
}