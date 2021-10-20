#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

void test(void);
int RoundUp(int toRound);
int RoundDown(int toRound);
int atm(int money);
bool test_atm(int money);

int main(void){
    test();
    int money;
    do{
        printf("How much money would you like? ");
        scanf("%d", &money);

        int ans = atm(money);
    }
    while(money%20!=0);

    return 0;
}

int atm(int money){
    if(money%20==0){
        int num = money/20;
        printf("OK, dispensing %d £20 notes...\n", num);
    }
    else if(money%10!=0){
        int rounded_up = RoundUp(money);
        int rounded_down = RoundDown(money);
        printf("I can give you %d or %d, try again\n", rounded_down, rounded_up);
    }
    return 0;
}

bool test_atm(int money){
    if(money%20==0){
        return true;
    }
    else if(money%10!=0){
        return false;
    }
}

int RoundUp(int toRound){
    if(toRound % 10 == 0){
        return toRound;
    }

    toRound = (10 - toRound % 10) + toRound;

    if(toRound%20!=0){
        toRound = RoundUp(toRound+1);
    }
    return toRound;
}

int RoundDown(int toRound){
    toRound = toRound - toRound % 10;

    if(toRound%20!=0){
        toRound = RoundDown(toRound-1);
    }
    return toRound;
}

void test(void){
    assert(RoundUp(26)==40);
    assert(RoundUp(154)==160);
    assert(RoundUp(1563)==1580);
    assert(RoundDown(26)==20);
    assert(RoundDown(158)==140);
    assert(RoundDown(1234)==1220);
    assert(test_atm(1280)==true);
    assert(test_atm(234880)==true);
    assert(test_atm(139)==false);
    assert(test_atm(241232)==false);
}