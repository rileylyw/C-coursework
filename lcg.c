#include <stdio.h>
#include <stdlib.h>

#define A 9
#define C 5
#define M 11

int seed = 0;

void lcg(void);

int main(void){
    lcg();
    return 0;
}

void lcg(void){
    do{
        seed = (A * seed + C) % M;
        printf("%d\n", seed);
    }
    while(seed!=0);
}

