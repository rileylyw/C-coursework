#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef enum typeofbet {oddeven = 1, particular = 2} typeofbet;
typedef enum oddoreven {odd = 1, even = 2} oddoreven;

int ran_num(void);
int roulette(void);
void test(void);

int main(void){
    int ans = roulette();
    return 0;
}

int ran_num(void){
    int ran_num;
    while(!(ran_num >= 0 && ran_num <= 35)){
        ran_num = rand();
    }
    return ran_num;
}

int roulette(void){
    int ran = ran_num();
    int bet;

    printf("How much would you like to bet? ");
    scanf("%d", &bet);

    typeofbet x;
    printf("\n1: Odd / even bet\n2: Particular number bet\nPlease select: ");
    scanf("%d", &x);

    if(x == 1){ //user chose odd or even bet
        oddoreven y;
        printf("\n1: Odd\n2: Even\nPlease select: ");
        scanf("%d", &y);

        if(ran == 0){
            printf("\nSorry, the answer is %d.\n", ran);
        }
        else if((y == 1 && ran % 2 == 1) || (y == 2 && ran % 2 == 0)){
            printf("\nCongrats, the answer is %d! You have won %d dollars.\n", ran, bet*2);
        }
        else{
            printf("Sorry, the answer is %d.\n", ran);
        }
        return 0;
    }

    if(x == 2){ //user chose particular input
        int z;
        printf("Please type in a number from 0-35: ");
        scanf("%d", &z);

        if(z == ran){
            printf("Congrats, the answer is %d! You have won %d dollars.\n", ran, bet*35);
        }
        else{
            printf("Sorry, the answer is %d.\n", ran);
        }
        return z;
    }
}

//void test(void){
//    assert(roulette(2, 2) == 0);
//}