#include <stdio.h>

int power(unsigned int a, unsigned int b);

int main(void){
    int x=2;
    int y=16;

    printf("%d^%d = %d\n", x, y, power(x,y));
}

int power(unsigned int a, unsigned int b){
    // 2^16
    if (b==0){
        return 1;
    }
    if (b==1){
        return a;
    }
    if (b%2==0){
        int r=power(a,b/2);
        return r*r;
    }
    return a*power(a, b-1);
}